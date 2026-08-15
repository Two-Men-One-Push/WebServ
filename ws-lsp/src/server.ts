import {
  createConnection,
  TextDocuments,
  Diagnostic,
  DiagnosticSeverity,
  ProposedFeatures,
  InitializeParams,
  TextDocumentSyncKind,
  InitializeResult,
} from 'vscode-languageserver/node';
import { TextDocument } from 'vscode-languageserver-textdocument';
import { parseDiagnosticLine } from './parse';
import { spawn } from 'child_process';
import { fileURLToPath, pathToFileURL } from 'url';
import * as fs from 'fs';
import * as path from 'path';

const connection = createConnection(ProposedFeatures.all);
const documents = new TextDocuments(TextDocument);

interface Settings {
  executablePath: string;
  args: string[];
  oneBasedLines: boolean;
  oneBasedColumns: boolean;
  runOn: 'change' | 'save';
  useTempFile: boolean;
  testArg: string;
}

let settings: Settings = {
  executablePath: '',
  args: [],
  oneBasedLines: true,
  oneBasedColumns: true,
  runOn: 'change',
  // false par défaut : l'analyseur doit voir le vrai fichier pour
  // résoudre les chemins d'inclusion relatifs correctement.
  useTempFile: false,
  // Le webserv supporte `-t` (ou `--test`) pour ne faire que le parsing
  // de la configuration sans démarrer le serveur.
  testArg: '-t',
};

// Pour chaque document ouvert : ensemble des URIs qui avaient des diagnostics
// lors de la dernière passe (permet d'effacer les fichiers inclus qui n'ont
// plus d'erreurs).
const previousDiagnosticUris = new Map<string, Set<string>>();

// Numéro de version de la dernière analyse par URI : une analyse plus récente
// invalide celles qui étaient en cours.
const validationTokens = new Map<string, number>();

connection.onInitialize((_params: InitializeParams): InitializeResult => {
  return {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
    },
  };
});

connection.onDidChangeConfiguration((change) => {
  const incoming = change.settings?.wsLanguageServer;
  if (incoming) settings = { ...settings, ...incoming };
  documents.all().forEach(validate);
});

// ---- Déclencheurs ----

const timers = new Map<string, NodeJS.Timeout>();

documents.onDidOpen((e) => validate(e.document));

documents.onDidChangeContent((e) => {
  if (settings.runOn !== 'change') return;
  const uri = e.document.uri;
  const prev = timers.get(uri);
  if (prev) clearTimeout(prev);
  timers.set(uri, setTimeout(() => {
    timers.delete(uri);
    void validate(e.document);
  }, 300));
});

documents.onDidSave((e) => validate(e.document));

documents.onDidClose((e) => {
  const uri = e.document.uri;
  const uris = previousDiagnosticUris.get(uri) ?? new Set();
  uris.add(uri);
  for (const u of uris) connection.sendDiagnostics({ uri: u, diagnostics: [] });
  previousDiagnosticUris.delete(uri);
  validationTokens.delete(uri);
});

// ---- Analyse ----

async function validate(doc: TextDocument): Promise<void> {
  if (!settings.executablePath) {
    connection.console.warn(
      'ws-lsp : "wsLanguageServer.executablePath" non configuré.'
    );
    return;
  }

  const token = (validationTokens.get(doc.uri) ?? 0) + 1;
  validationTokens.set(doc.uri, token);

  // Répertoire du vrai fichier — sert à résoudre les chemins relatifs
  // de la sortie ET à définir le cwd du processus analyseur.
  let docDir: string;
  let realPath: string;
  try {
    realPath = fileURLToPath(doc.uri);
    docDir   = path.dirname(realPath);
  } catch {
    connection.console.error('ws-lsp : URI non supportée : ' + doc.uri);
    return;
  }

  let targetPath: string;
  let tempPath: string | undefined;

  if (settings.useTempFile) {
    // On place le fichier temporaire DANS le même répertoire que le vrai
    // fichier (et non dans /tmp/) pour que l'analyseur trouve les includes.
    const key = Buffer.from(doc.uri).toString('hex').slice(0, 12);
    tempPath   = path.join(docDir, `.ws-lsp-tmp-${key}.ws`);
    try {
      fs.writeFileSync(tempPath, doc.getText(), 'utf8');
    } catch (err) {
      connection.console.error(`ws-lsp : échec écriture fichier temporaire : ${err}`);
      return;
    }
    targetPath = tempPath;
  } else {
    // Mode normal : l'analyseur reçoit directement le vrai fichier.
    // Nécessite une sauvegarde pour voir les changements en live.
    targetPath = realPath;
  }

  const output = await runAnalyzer(targetPath, docDir);

  if (tempPath) {
    fs.unlink(tempPath, () => { /* best-effort */ });
  }

  // Une analyse plus récente a remplacé celle-ci : on jette les résultats.
  if (validationTokens.get(doc.uri) !== token) return;

  // ---- Répartition des diagnostics par URI cible ----
  const diagnosticsMap = new Map<string, Diagnostic[]>();

  for (const rawLine of output.split(/\r?\n/)) {
    const parsed = parseDiagnosticLine(rawLine);
    if (!parsed) continue;

    const targetUri = resolveUri(parsed.file, doc.uri, tempPath, docDir);

    let lineNum = parsed.line;
    let colNum  = parsed.col;
    if (settings.oneBasedLines)   lineNum = Math.max(0, lineNum - 1);
    if (settings.oneBasedColumns) colNum  = Math.max(0, colNum  - 1);

    const range = buildRange(targetUri, doc, lineNum, colNum);

    if (!diagnosticsMap.has(targetUri)) diagnosticsMap.set(targetUri, []);
    diagnosticsMap.get(targetUri)!.push({
      severity: detectSeverity(parsed.message),
      range,
      message: parsed.message,
      source: 'ws',
    });
  }

  // ---- Envoi ----

  // Efface les anciens fichiers inclus qui n'ont plus d'erreurs
  const prevUris = previousDiagnosticUris.get(doc.uri) ?? new Set<string>();
  for (const prevUri of prevUris) {
    if (!diagnosticsMap.has(prevUri)) {
      connection.sendDiagnostics({ uri: prevUri, diagnostics: [] });
    }
  }

  // Toujours envoyer un tableau (potentiellement vide) pour le doc courant
  if (!diagnosticsMap.has(doc.uri)) diagnosticsMap.set(doc.uri, []);

  const newUris = new Set<string>();
  for (const [uri, diagnostics] of diagnosticsMap) {
    connection.sendDiagnostics({ uri, diagnostics });
    if (diagnostics.length > 0) newUris.add(uri);
  }
  previousDiagnosticUris.set(doc.uri, newUris);
}

// ---- Helpers ----

/**
 * Construit les arguments de l'analyseur :
 *   [args de l'utilisateur] [testArg] <fichier>
 * L'option de test est ajoutée automatiquement pour que le webserv ne fasse
 * que parser la configuration sans démarrer le serveur.
 */
function analyzerArgs(filePath: string): string[] {
  const testArg = settings.testArg.trim();
  const args = [...settings.args];
  if (testArg && !args.includes(testArg)) {
    args.push(testArg);
  }
  args.push(filePath);
  return args;
}

/**
 * Convertit la partie "fichier" d'une ligne d'erreur en URI VSCode.
 *
 * Priorité :
 *  1. Vide → fichier courant
 *  2. Égal au fichier temporaire → fichier courant
 *  3. Chemin absolu → pathToFileURL directement
 *  4. Chemin relatif → résolu depuis docDir (= cwd de l'analyseur)
 */
function resolveUri(
  filePart: string,
  docUri: string,
  tempPath: string | undefined,
  docDir: string
): string {
  const trimmed = filePart.trim();
  if (!trimmed) return docUri;

  // Normalise les backslashes Windows en séparateurs natifs
  const normalized = trimmed.replace(/\\/g, path.sep);

  // Correspond au fichier temporaire (par nom complet ou juste le basename)
  if (tempPath) {
    if (normalized === tempPath || normalized === path.basename(tempPath)) {
      return docUri;
    }
  }

  // Chemin absolu
  if (path.isAbsolute(normalized)) {
    try { return pathToFileURL(normalized).href; } catch { return docUri; }
  }

  // Chemin relatif : résolu depuis docDir qui est aussi le cwd du processus.
  // Ex : "test.wsx" → "<docDir>/test.wsx"
  //      "include/utils.ws" → "<docDir>/include/utils.ws"
  try {
    return pathToFileURL(path.resolve(docDir, normalized)).href;
  } catch {
    return docUri;
  }
}

/**
 * Construit la plage à souligner en lisant le contenu du fichier cible.
 * Si le fichier n'est pas ouvert dans l'éditeur, on le lit depuis le disque.
 */
function buildRange(
  uri: string,
  currentDoc: TextDocument,
  lineNum: number,
  colNum: number
): Diagnostic['range'] {
  let lineText = '';

  if (uri === currentDoc.uri) {
    lineText = currentDoc.getText().split(/\r?\n/)[lineNum] ?? '';
  } else {
    try {
      lineText = fs.readFileSync(fileURLToPath(uri), 'utf8').split(/\r?\n/)[lineNum] ?? '';
    } catch { /* fichier illisible : un seul caractère souligné */ }
  }

  const word = /^\S+/.exec(lineText.slice(colNum));
  const endChar = word ? colNum + word[0].length : colNum + 1;

  return {
    start: { line: lineNum, character: colNum },
    end:   { line: lineNum, character: endChar },
  };
}

/**
 * Lance l'analyseur avec cwd = docDir.
 * Cela garantit que les chemins relatifs dans la sortie sont bien
 * relatifs au répertoire du fichier analysé (comportement identique
 * à un appel manuel depuis le terminal dans ce répertoire).
 *
 * Un garde-fou de 5 s tue le processus : si le binaire n'a pas reçu
 * l'option de test, il ne faut pas bloquer l'extension sur un serveur
 * qui tournerait indéfiniment.
 */
function runAnalyzer(filePath: string, cwd: string): Promise<string> {
  return new Promise((resolve) => {
    const args  = analyzerArgs(filePath);
    const child = spawn(settings.executablePath, args, {
      shell: false,
      cwd,          // ← clé du correctif : même répertoire que l'appel manuel
    });

    let stdout = '';
    let stderr = '';
    let settled = false;
    const settle = (value: string) => {
      if (settled) return;
      settled = true;
      resolve(value);
    };

    child.stdout.on('data', (d) => (stdout += d.toString()));
    child.stderr.on('data', (d) => (stderr += d.toString()));

    const timer = setTimeout(() => {
      connection.console.warn(
        'ws-lsp : analyse interrompue après 5 s, processus tué. ' +
        'Vérifiez que le binaire reçoit bien l\'option de test ("' +
        settings.testArg + '").'
      );
      child.kill('SIGKILL');
      settle(stdout + '\n' + stderr);
    }, 5000);

    child.on('error', (err) => {
      clearTimeout(timer);
      connection.console.error(
        `ws-lsp : impossible de lancer "${settings.executablePath}" : ${err.message}`
      );
      settle('');
    });

    child.on('close', () => {
      clearTimeout(timer);
      settle(stdout + '\n' + stderr);
    });
  });
}

function detectSeverity(message: string): DiagnosticSeverity {
  const m = message.toLowerCase();
  if (m.includes('warning') || m.includes('warn') || m.startsWith('avertissement'))
    return DiagnosticSeverity.Warning;
  if (m.includes('info') || m.includes('note'))
    return DiagnosticSeverity.Information;
  return DiagnosticSeverity.Error;
}

documents.listen(connection);
connection.listen();
