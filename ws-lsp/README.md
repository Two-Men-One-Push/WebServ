# WS Language Server

Extension VS Code fournissant des diagnostics pour les fichiers de configuration
`.ws` du projet WebServ, en s'appuyant sur le binaire `webserv` lui-même.

## Fonctionnement

L'extension est un vrai language server (client dans `src/extension.ts`,
serveur dans `src/server.ts`). À chaque modification ou sauvegarde d'un
fichier `.ws`, elle lance `webserv -t <fichier>` :

- `-t` (ou `--test`) fait parser et valider la configuration **sans démarrer
  le serveur** ;
- la sortie du serveur est parsée ligne par ligne (`fichier:ligne:colonne:
  error: message`) ;
- les erreurs sont affichées **aussi bien sur le fichier analysé que sur les
  fichiers inclus** via `include` : le serveur émet déjà le chemin du fichier
  fautif, l'extension le résout et reporte le diagnostic dans le bon onglet.

L'option de test est ajoutée automatiquement aux arguments, donc l'extension
ne bloque jamais sur un serveur en cours d'exécution. Un garde-fou de 5 s tue
l'analyse si le binaire ne répond pas.

## Configuration

Le réglage `wsLanguageServer.executablePath` est **obligatoire** : pointez-le
vers votre binaire `webserv` (il n'est pas embarqué dans l'extension).

| Réglage | Valeurs | Défaut | Description |
| --- | --- | --- | --- |
| `wsLanguageServer.executablePath` | chemin | *(vide)* | Binaire analysant les `.ws` |
| `wsLanguageServer.args` | tableau | `[]` | Arguments supplémentaires passés avant le fichier |
| `wsLanguageServer.testArg` | chaîne | `-t` | Option de parsing sans serveur. Vide = désactivé |
| `wsLanguageServer.runOn` | `change` / `save` | `change` | Quand lancer l'analyse (avec debounce de 300 ms) |
| `wsLanguageServer.useTempFile` | booléen | `true` | Analyser le buffer non sauvegardé via un fichier temporaire (placé dans le même dossier, donc les `include` relatifs fonctionnent) |
| `wsLanguageServer.oneBasedLines` | booléen | `true` | Index de ligne du binaire (1 = conventionnel) |
| `wsLanguageServer.oneBasedColumns` | booléen | `true` | Index de colonne du binaire |

## Format d'entrée attendu du binaire

Le LSP parse les lignes au format :

```
<fichier>:<ligne>:<colonne>: error: <message>
```

Le serveur WebServ écrit ce format avec un préfixe de log
(`AAAA-MM-JJ HH:MM:SS: error: …`) avec des couleurs ANSI ; les deux formes
sont acceptées. Toute autre ligne de sortie (logs généraux, etc.) est ignorée.

## Développement

```bash
npm install
npm run compile      # tsc -p ./
npm run watch        # recompilation continue
```

Puis lancer l'extension dans VS Code : `F5` dans ce dossier (le lancement
compile d'abord via `preLaunchTask`). Pour empaqueter :

```bash
npx vsce package
```
