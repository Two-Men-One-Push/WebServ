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
- per-server and per-location configuration overrides

The repository also contains example content under `example/` to help with manual
testing, including a sample configuration file and a basic document root.

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

`make test_env` creates a small sample directory tree used for manual tests.

### Run
Launch the server with a configuration file:

```bash
./webserv example/test.ws
```

The project is designed for Linux because it relies on `epoll`.

### Configuration overview
The configuration file is parsed in three stages: lexer, parser, and semantic
analysis. From a language-design perspective, the file defines a small
configuration grammar with hierarchical scopes and inherited defaults.
The syntax uses nested blocks such as:

```conf
http {
	include mime.types;
	max_body_size 1000000;

	server {
		listen 8080;
		root ./www;
		index index.html;
		autoindex off;

		location /images {
			autoindex on;
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
timeout_directive      ::= 'timeout' <seconds>

type_rule              ::= <mime-type> <extension>+
```

Directive notes:
- `include` can appear in every scope and expands external configuration files before semantic analysis.
- `listen` accepts either `<port>` or `<host>:<port>`.
- `error_page` maps one or more status codes to an error page, optionally with a different response status.
- `cgi` associates one or more file extensions with an interpreter path.
- `editable` enables upload/delete handling for the current scope.
- `types` defines MIME type mappings used when serving static files.
- `location` blocks may be nested, and nested locations inherit from their parent scope.

The implemented HTTP methods are `GET`, `POST`, and `DELETE`.

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
- Keep-alive aware request/response handling

## Repository Layout
- `src/` contains the implementation
- `include/` contains shared headers and defaults
- `example/` contains sample configuration and test assets
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
