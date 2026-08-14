*This project has been created as part of the 42 curriculum by ebini and ethebaul.*

# WebServ

## Description
WebServ is a small HTTP/1.1 web server written in C++98 for the 42 curriculum.
Its goal is to parse a custom configuration file, open one or more listening
sockets, and serve requests with an event-driven architecture based on `epoll`.

The server supports:
- static file serving
- directory listing through autoindex
- custom error pages
- redirections
- CGI execution
- file upload and delete flows when a location is marked editable
- HEAD requests, answered without a body
- per-server and per-location configuration overrides
- configurable connection timeouts

The repository also contains sample content for manual testing under `example/`:
a configuration file (`test.ws`), a MIME type mapping file (`mime.types`),
and a document root (`www/`). An optional VS Code extension providing language
support for the configuration files lives under `ws-lsp/`.

## Instructions

### Build
The project is built with the provided `Makefile`:

```bash
make
```

Other useful targets:

```bash
make clean
make fclean
make re
make clangd
make test_env
```

`make clangd` generates a `compile_flags.txt` for clangd. `make test_env`
creates a small sample directory tree used for manual tests.

### Run
Launch the server with a configuration file:

```bash
./webserv example/test.ws
```

The project is designed for Linux because it relies on `epoll`.

### Command-line options
The server accepts several options before the configuration file:

```text
Usage: ./webserv [OPTIONS] <config_file>

  -h, --help               Display this message and exit
  -s, --silent             Suppress non-error output
  -t, --test               Test the configuration and exit
  -v                       Increase verbosity
      --verbose <LEVEL>    Set verbosity level: silent, error, warn, info, debug
```

`-v` sets the level to `info`, and `-vv` (repeated `v` characters, e.g. `-vv`)
sets it to `debug`. The default level is `warn`. `-t` parses and validates the
configuration, then exits without starting the server, which is useful for
checking a config file in CI or before a restart.

### Configuration overview
The configuration file is parsed in three stages: lexer, parser, and semantic
analysis. From a language-design perspective, the file defines a small
configuration grammar with hierarchical scopes and inherited defaults.
The syntax uses nested blocks such as:

```conf
http {
	include mime.types;
	error_page 404 =404 /404.html;
	error_page 502 =502 /errorPages/502.html;

	server {
		listen localhost:6969;
		timeout 1000s;
		root ./www;
		autoindex on;

		cgi py /usr/bin/python3;

		location cgi {
			editable on;
		}
	}
}
```

The active directives are organized by scope:

- **http**
	- `include`
	- `max_body_size`
	- `error_page`
	- `cgi`
	- `types`
	- **server**
		- `include`
		- `listen`
		- `root`
		- `index`
		- `error_page`
		- `max_body_size`
		- `allow_methods`
		- `autoindex`
		- `redirect`
		- `cgi`
		- `editable`
		- `types`
		- `timeout`
		- **location**
			- `include`
			- `root`
			- `index`
			- `error_page`
			- `max_body_size`
			- `allow_methods`
			- `autoindex`
			- `redirect`
			- `cgi`
			- `editable`
			- `types`
			- **location** (nested locations are allowed)

Grammar sketch (informal EBNF):

```ebnf
config          ::= http_block

http_block      ::= 'http' '{' http_directive* server_block+ '}'
server_block    ::= 'server' '{' server_directive* location_block* '}'
location_block  ::= 'location' <path> '{' location_directive* location_block* '}'

http_directive  ::= include_directive
				  | max_body_size_directive
				  | error_page_directive
				  | cgi_directive
				  | types_directive

server_directive ::= include_directive
				   | listen_directive
				   | root_directive
				   | index_directive
				   | error_page_directive
				   | max_body_size_directive
				   | allow_methods_directive
				   | autoindex_directive
				   | redirect_directive
				   | cgi_directive
				   | editable_directive
				   | types_directive
				   | timeout_directive

location_directive ::= include_directive
					 | root_directive
					 | index_directive
					 | error_page_directive
					 | max_body_size_directive
					 | allow_methods_directive
					 | autoindex_directive
					 | redirect_directive
					 | cgi_directive
					 | editable_directive
					 | types_directive

include_directive      ::= 'include' <file>+
listen_directive       ::= 'listen' <host:port>
root_directive         ::= 'root' <path>
index_directive        ::= 'index' <file>+
error_page_directive   ::= 'error_page' <status>+ [ '=' <status> ] <path>
max_body_size_directive::= 'max_body_size' <size>
allow_methods_directive::= 'allow_methods' <method>+
autoindex_directive    ::= 'autoindex' ('on' | 'off')
redirect_directive     ::= 'redirect' <url>
cgi_directive          ::= 'cgi' <extension>+ <interpreter>
editable_directive     ::= 'editable' ('on' | 'off')
types_directive        ::= 'types' '{' type_rule+ '}'
timeout_directive      ::= 'timeout' <duration>

type_rule              ::= <mime-type> <extension>+
```

Directive notes:
- `include` can appear in every scope and expands external configuration files before semantic analysis. Circular includes are detected and reported.
- `listen` accepts either `<port>` or `<host>:<port>`. Several `listen` directives may be given per server.
- `error_page` maps one or more status codes to an error page, optionally with a different response status.
- `cgi` associates one or more file extensions with an interpreter path.
- `editable` enables upload/delete handling for the current scope.
- `types` defines MIME type mappings used when serving static files.
- `location` blocks may be nested, and nested locations inherit from their parent scope.
- `max_body_size` accepts plain bytes or a size with a suffix: `k`/`ko`/`kb`, `m`/`mo`/`mb`, `g`/`go`/`gb` (powers of 1000), or `ki`/`kio`/`kib`, `mi`/`mio`/`mib`, `gi`/`gio`/`gib` (powers of 1024).
- `timeout` is a server-scope directive. Values accept an optional suffix: `s` (seconds, default), `m` (minutes), `h` (hours). The default timeout is 8 seconds.

The implemented HTTP methods are `GET`, `HEAD`, `POST`, and `DELETE`. `HEAD`
responses carry the same headers as the corresponding `GET` but no body.
Requests using other methods are answered with `501 Not Implemented`.

## Features
- Lexer, parser, preprocessor, and semantic validation for the custom config language
- Multi-server support with multiple listen addresses and ports
- Static content serving from a configurable document root
- MIME type mapping from a configuration file such as `example/mime.types`
- Custom error pages with fallback generation when no file is provided
- Directory listings when `autoindex on` is enabled
- CGI execution through configured interpreters
- Request routing by location, with inherited defaults from parent blocks
- Upload and delete handling for editable locations
- HEAD requests handled without a body
- Configurable connection timeouts
- Keep-alive aware request/response handling
- Hierarchical logger with levels from `silent` to `debug`, and CLI verbosity flags
- Configuration test mode (`-t`) that validates a config without starting the server

## WS Language Server (ws-lsp)
`ws-lsp/` contains a VS Code extension that provides diagnostics for `.ws`
configuration files by running the `webserv` binary itself. It is a real
language server (client in `ws-lsp/src/extension.ts`, server in
`ws-lsp/src/server.ts`) and reports errors on the file being edited **as well
as on included files**: the server emits the offending file's path in its
diagnostic output, and the extension resolves it to the right document.

The analyzer is invoked as `executablePath [-t] [extra args] <file>`. The
`-t` option (`wsLanguageServer.testArg`) is added automatically so the config
is only parsed and validated without starting the server; a 5-second watchdog
kills the process if it ever hangs.

Build and install:
```bash
cd ws-lsp
npm install
npm run compile
```

Then run the extension in VS Code (`F5` in the extension workspace) or package
it with `vsce package`. Once installed, point the `wsLanguageServer.executablePath`
setting at your `webserv` binary (for example `./webserv`), because it does not
ship with one. Its stderr/stdout is parsed for `path:line:column: error: message`
lines, with or without the timestamp/level prefix added by the Logger.

Useful settings:
- `wsLanguageServer.executablePath` — path to the `webserv` binary (required)
- `wsLanguageServer.testArg` — test-only flag added automatically (`-t` by default, empty to disable)
- `wsLanguageServer.runOn` — analyze on every `change` (debounced) or on `save`
- `wsLanguageServer.useTempFile` — analyze the unsaved buffer via a temporary file (placed next to the real file, so relative `include`s still resolve)
- `wsLanguageServer.oneBasedLines` / `oneBasedColumns` — index base of the analyzer output

## Repository Layout
- `src/` contains the implementation
- `include/` contains shared headers and defaults
- `example/` contains the sample files: `test.ws` (configuration), `mime.types` (MIME mappings), and `www/` (document root)
- `ws-lsp/` contains the optional VS Code language-support extension

## Resources
Useful references for this project:
- MDN Web Docs: HTTP overview and header semantics
- RFC 7230: Hypertext Transfer Protocol (HTTP/1.1) message syntax and routing
- RFC 7231: HTTP/1.1 semantics and content negotiation
- CGI 1.1 specification
- Linux man pages for `epoll`, `socket`, `bind`, `listen`, `accept`, and `getaddrinfo`
- 42 subject guidelines for the WebServ project

### AI usage
AI was used to help draft and structure this README after manually inspecting the
repository. It helped summarize the implemented features, organize the build and
run instructions, and improve the English wording. The project code itself was
not generated by AI.
