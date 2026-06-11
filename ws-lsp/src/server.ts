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
import { spawn } from 'child_process';
import { fileURLToPath } from 'url';
import * as fs from 'fs';
import * as os from 'os';
import * as path from 'path';

// Connexion vers VSCode + gestion des documents ouverts
const connection = createConnection(ProposedFeatures.all);
const documents = new TextDocuments(TextDocument);

interface Settings {
  executablePath: string;
  args: string[];
  oneBasedLines: boolean;
  oneBasedColumns: boolean;
  runOn: 'change' | 'save';
  useTempFile: boolean;
}

let settings: Settings = {
  executablePath: '',
  args: [],
  oneBasedLines: true,
  oneBasedColumns: true,
  runOn: 'change',
  useTempFile: true,
};

connection.onInitialize((_params: InitializeParams): InitializeResult => {
  return {
    capabilities: {
      textDocumentSync: TextDocumentSyncKind.Incremental,
    },
  };
});

// Recoit la configuration (et ses mises a jour) depuis VSCode
connection.onDidChangeConfiguration((change) => {
  const incoming = change.settings?.wsLanguageServer;
  if (incoming) {
    settings = { ...settings, ...incoming };
  }
  // On relance l'analyse sur tous les documents ouverts
  documents.all().forEach(validate);
});

// ---- Declencheurs ----

// Anti-rebond : evite de lancer le programme a chaque touche frappee
const timers = new Map<string, NodeJS.Timeout>();

documents.onDidOpen((e) => {
  validate(e.document);
});

documents.onDidChangeContent((e) => {
  if (settings.runOn !== 'change') {
    return;
  }
  const uri = e.document.uri;
  const previous = timers.get(uri);
  if (previous) {
    clearTimeout(previous);
  }
  timers.set(
    uri,
    setTimeout(() => {
      timers.delete(uri);
      validate(e.document);
    }, 300)
  );
});

documents.onDidSave((e) => {
  validate(e.document);
});

documents.onDidClose((e) => {
  // On efface les diagnostics quand le fichier est ferme
  connection.sendDiagnostics({ uri: e.document.uri, diagnostics: [] });
});

// ---- Analyse ----

// Capture: <fichier>:<ligne>:<colonne><espace ou ":">  <message>
// Le ".*?" non-gourmand gere les chemins Windows type C:\dossier\fichier.ws
const lineRegex = /^(.*?):(\d+):(\d+)(?:\s+|:\s*)(.*)$/;

async function validate(doc: TextDocument): Promise<void> {
  if (!settings.executablePath) {
    connection.console.warn(
      'ws-lsp : aucun "wsLanguageServer.executablePath" configure dans les parametres.'
    );
    return;
  }

  // On determine le fichier a analyser
  let targetPath: string;
  let tempPath: string | undefined;

  if (settings.useTempFile) {
    // On ecrit le contenu courant (meme non sauvegarde) dans un fichier temporaire
    const key = Buffer.from(doc.uri).toString('hex');
    tempPath = path.join(os.tmpdir(), `ws-lsp-${key}.ws`);
    try {
      fs.writeFileSync(tempPath, doc.getText(), 'utf8');
    } catch (err) {
      connection.console.error(`ws-lsp : echec ecriture fichier temporaire : ${err}`);
      return;
    }
    targetPath = tempPath;
  } else {
    // On utilise le vrai fichier sur disque (necessite qu'il soit sauvegarde)
    try {
      targetPath = fileURLToPath(doc.uri);
    } catch {
      return; // Pas un fichier local
    }
  }

  const output = await runAnalyzer(targetPath);

  if (tempPath) {
    fs.unlink(tempPath, () => {
      /* nettoyage best-effort */
    });
  }

  const text = doc.getText();
  const lines = text.split(/\r?\n/);
  const diagnostics: Diagnostic[] = [];

  for (const rawLine of output.split(/\r?\n/)) {
    const trimmed = rawLine.trim();
    if (!trimmed) {
      continue;
    }
    const match = lineRegex.exec(trimmed);
    if (!match) {
      continue; // ligne de sortie qui ne correspond pas au format attendu
    }

    const [, , lineStr, colStr, message] = match;
    let lineNum = parseInt(lineStr, 10);
    let colNum = parseInt(colStr, 10);

    if (settings.oneBasedLines) {
      lineNum = Math.max(0, lineNum - 1);
    }
    if (settings.oneBasedColumns) {
      colNum = Math.max(0, colNum - 1);
    }

    // On souligne le "mot" qui commence a la colonne, sinon au moins un caractere
    const lineText = lines[lineNum] ?? '';
    let endChar = colNum + 1;
    const rest = lineText.slice(colNum);
    const word = /^\S+/.exec(rest);
    if (word) {
      endChar = colNum + word[0].length;
    }

    diagnostics.push({
      severity: detectSeverity(message),
      range: {
        start: { line: lineNum, character: colNum },
        end: { line: lineNum, character: endChar },
      },
      message,
      source: 'ws',
    });
  }

  connection.sendDiagnostics({ uri: doc.uri, diagnostics });
}

function runAnalyzer(filePath: string): Promise<string> {
  return new Promise((resolve) => {
    const args = [...settings.args, filePath];
    const child = spawn(settings.executablePath, args, { shell: false });

    let stdout = '';
    let stderr = '';

    child.stdout.on('data', (d) => (stdout += d.toString()));
    child.stderr.on('data', (d) => (stderr += d.toString()));

    child.on('error', (err) => {
      connection.console.error(
        `ws-lsp : impossible de lancer l'analyseur "${settings.executablePath}" : ${err.message}`
      );
      resolve('');
    });

    // On combine stdout + stderr : certains programmes ecrivent les erreurs sur l'un ou l'autre
    child.on('close', () => resolve(stdout + '\n' + stderr));
  });
}

function detectSeverity(message: string): DiagnosticSeverity {
  const m = message.toLowerCase();
  if (m.includes('warning') || m.includes('warn') || m.startsWith('avertissement')) {
    return DiagnosticSeverity.Warning;
  }
  if (m.includes('info') || m.includes('note')) {
    return DiagnosticSeverity.Information;
  }
  return DiagnosticSeverity.Error;
}

documents.listen(connection);
connection.listen();