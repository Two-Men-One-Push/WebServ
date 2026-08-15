// Parsing des diagnostics émis par le serveur webserv.
//
// Le webserv écrit ses messages sur stderr via son Logger, au format :
//   <timestamp>: <niveau coloré ANSI>: <contenu>
// et les diagnostics suivent la forme :
//   <fichier>:<ligne>:<colonne>: error: <message>
// Le fichier peut être le fichier analysé OU un fichier inclus
// (les erreurs de parse des includes pointent déjà vers leur propre fichier).

const ANSI_ESCAPE = /\u001B\[[0-9;]*[A-Za-z]/g;

// Préfixe de ligne du logger : timestamp + niveau (éventuellement coloré).
// Le timestamp suit le format strftime "%Y-%m-%d %H:%M:%S" (voir Logger::LogStream).
const LOG_LINE_PREFIX = /^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}:\s*(?:error|warn|info|debug)\s*:\s*(.+)$/i;

// Capture : <chemin>:<ligne>:<colonne>[<séparateur>]<message>
// .*? non-gourmand → gère les chemins Windows (C:\...) et les noms sans séparateur
const DIAGNOSTIC_LINE = /^(.*?):(\d+):(\d+)(?::|\s+)(.+)$/;

// Ligne "path:line:col: error: message" : on retire le préfixe "error: "
// (éventuellement précédé d'espaces) pour ne garder que le message.
const MESSAGE_LEVEL_PREFIX = /^\s*(?:error|warning|warn|info|note)\s*:\s*/i;

export function stripAnsi(text: string): string {
  return text.replace(ANSI_ESCAPE, '');
}

export interface ParsedDiagnostic {
  file: string;
  line: number;
  col: number;
  message: string;
}

/**
 * Parse une ligne de sortie du serveur et retourne un diagnostic si la ligne
 * correspond à un "path:line:col: message". Retourne null pour les autres
 * lignes (logs généraux, "configuration has errors...", lignes vides).
 *
 * Accepte à la fois le format avec le préfixe du Logger (timestamp + niveau)
 * et le format brut sans préfixe.
 */
export function parseDiagnosticLine(rawLine: string): ParsedDiagnostic | null {
  const cleaned = stripAnsi(rawLine.trim());
  if (!cleaned) return null;

  const content = LOG_LINE_PREFIX.exec(cleaned)?.[1] ?? cleaned;

  const match = DIAGNOSTIC_LINE.exec(content);
  if (!match) return null;

  const [, file, lineStr, colStr, rawMessage] = match;
  const message = rawMessage.replace(MESSAGE_LEVEL_PREFIX, '').trim();

  return {
    file,
    line: parseInt(lineStr, 10),
    col: parseInt(colStr, 10),
    message,
  };
}
