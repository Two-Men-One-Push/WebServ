import * as path from 'path';
import { ExtensionContext } from 'vscode';
import {
  LanguageClient,
  LanguageClientOptions,
  ServerOptions,
  TransportKind,
} from 'vscode-languageclient/node';

let client: LanguageClient;

export function activate(context: ExtensionContext) {
  // Le serveur est compile dans out/server.js
  const serverModule = context.asAbsolutePath(path.join('out', 'server.js'));

  // Le serveur tourne dans un processus Node separe (out-of-process), comme un vrai LSP.
  const serverOptions: ServerOptions = {
    run: { module: serverModule, transport: TransportKind.ipc },
    debug: {
      module: serverModule,
      transport: TransportKind.ipc,
      options: { execArgv: ['--nolazy', '--inspect=6009'] },
    },
  };

  const clientOptions: LanguageClientOptions = {
    // On ne s'occupe QUE des fichiers .ws (langage "ws" declare dans package.json)
    documentSelector: [{ scheme: 'file', language: 'ws' }],
    synchronize: {
      // Le serveur recevra les changements de cette section de configuration
      configurationSection: 'wsLanguageServer',
    },
  };

  client = new LanguageClient(
    'wsLanguageServer',
    'WS Language Server',
    serverOptions,
    clientOptions
  );

  client.start();
}

export function deactivate(): Thenable<void> | undefined {
  return client?.stop();
}