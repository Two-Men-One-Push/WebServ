#include "Semantic.hpp"
#include "Utils.hpp"
#include "Word.hpp"
#include <algorithm>

Semantic::Semantic()
{
}

Semantic::~Semantic()
{
}

bool	Semantic::checkShape(const Directive &d, ArgShape args, BodyShape body, DiagnosticContext &diag)
{
	bool		canProcess = true;
	std::string	n = d.name().rawContent();

	if (args == ARGS_FORBIDDEN && !d.args().empty())
	{
		for (std::vector<Word>::const_iterator it = d.args().begin(); it != d.args().end(); ++it)
			diag.report("'" + n + "' directive does not take arguments", *it);
	}
	else if (args == ARGS_AT_LEAST_ONE && d.args().empty())
	{
		diag.report("'" + n + "' directive requires at least one argument", d);
		canProcess = false;
	}
	else if (args == ARGS_AT_LEAST_TWO && d.args().size() < 2)
	{
		diag.report("'" + n + "' directive requires at least two arguments", d);
		canProcess = false;
	}
	else if (args == ARGS_EXACT_ONE && d.args().size() != 1)
	{
		diag.report("'" + n + "' directive requires exactly one argument", d);
		if (d.args().empty())
			canProcess = false;
	}
	else if (args == ARGS_EXACT_TWO && d.args().size() != 2)
	{
		diag.report("'" + n + "' directive requires exactly two arguments", d);
		if (d.args().size() < 2)
			canProcess = false;
	}
	if (body == BODY_FORBIDDEN && d.hasBody())
		diag.report("'" + n + "' directive cannot have a body", d.blockErrorInfo());
	else if (body == BODY_REQUIRED && !d.hasBody())
	{
		diag.report("'" + n + "' directive requires a body", d);
		canProcess = false;
	}
	return canProcess;
}

Config	Semantic::analyseAST(const AST &ast, DiagnosticContext &diag)
{
	Config	config;

	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "http")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				config.http() = analyseHttp(it->children(), diag);
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return config;
}

Http	Semantic::analyseHttp(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	Http	http;
	bool	clientMaxBodySizeDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "client_max_body_size")
		{
			if (clientMaxBodySizeDefined)
				diag.report("duplicate directive 'client_max_body_size'", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
			{
				size_t size;
				if (!parseSize(it->args().front().rawContent(), size))
					diag.report("invalid size '" + it->args().front().rawContent() + "'", it->args().front());
				else
					http.clientMaxBodySize() = size;
			}
			clientMaxBodySizeDefined = true;
		}
		else if (name == "error_page")
		{
			if (checkShape(*it, ARGS_AT_LEAST_TWO, BODY_FORBIDDEN, diag))
			{
				std::string							path = it->args().back().rawContent();
				std::vector<Word>::const_iterator	penultimate = --(--it->args().end());
				int									response_code = 0;
				if (penultimate->rawContent()[0] == '=')
				{
					if (!parseInt(penultimate->rawContent().substr(1), response_code))
						diag.report("invalid response code '" + penultimate->rawContent().substr(1) + "'", *penultimate);
					if (response_code < 100 || response_code > 599)
						diag.report("response code must be between 100 and 599", *penultimate);
				}
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != penultimate; ++arg)
				{
					int code;
					if (!parseInt(arg->rawContent(), code))
					{
						diag.report("invalid response code '" + arg->rawContent() + "'", *arg);
						continue;
					}
					if (code < 100 || code > 599)
					{
						diag.report("response code must be between 100 and 599", *arg);
						continue;
					}
					if (http.errorPages().find(code) != http.errorPages().end())
					{
						diag.report("duplicate error page for response code '" + arg->rawContent() + "'", *arg);
						continue;
					}
					http.errorPages()[code] = std::make_pair(response_code, path);
				}
			}
		}
		else if (name == "types")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				analyseTypes(it->children(), http.types(), diag);
		}
		else if (name == "server")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				http.servers().push_back(analyseServer(it->children(), http, diag));
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return http;
}

Server	Semantic::analyseServer(const std::list<Directive> &directives, Http &http, DiagnosticContext &diag)
{
	Server	server(http);
	bool	listenDefined = false;
	bool	serverNameDefined = false;
	bool	rootDefined = false;
	bool	indexDefined = false;
	bool	errorPageDefined = false;
	bool	clientMaxBodySizeDefined = false;
	bool	typesDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "listen")
		{
			if (listenDefined)
				diag.report("duplicate directive 'listen'", *it);
			else if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					bool	duplicate = false;
					int		port;
					if (!parseInt(arg->rawContent(), port))
					{
						diag.report("invalid port number '" + arg->rawContent() + "'", *arg);
						continue;
					}
					if (port < 0 || port > 65535)
					{
						diag.report("port number must be between 0 and 65535", *arg);
						continue;
					}
					for (std::vector<Server>::const_iterator serv = http.servers().begin(); serv != http.servers().end(); ++serv)
					{
						if (std::find(serv->listen().begin(), serv->listen().end(), port) != serv->listen().end())
						{
							diag.report("duplicate port number '" + arg->rawContent() + "'", *arg);
							duplicate = true;
							break;
						}
					}
					if (!duplicate)
						server.listen().push_back(port);
				}
			}
			listenDefined = true;
		}
		else if (name == "server_name")
		{
			if (serverNameDefined)
				diag.report("duplicate directive 'server_name'", *it);
			else if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					const std::string serverName = arg->rawContent();
					bool duplicate = false;
					for (std::vector<Server>::const_iterator serv = http.servers().begin(); serv != http.servers().end(); ++serv)
					{
						if (std::find(serv->serverNames().begin(), serv->serverNames().end(), serverName) != serv->serverNames().end())
						{
							diag.report("duplicate server name '" + serverName + "'", *arg);
							duplicate = true;
							break;
						}
					}
					if (!duplicate)
						server.serverNames().push_back(serverName);
				}
			}
			serverNameDefined = true;
		}
		else if (name == "root")
		{
			if (rootDefined)
				diag.report("duplicate directive 'root'", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
				server.root() = it->args().front().rawContent();
			rootDefined = true;
		}
		else if (name == "index")
		{
			if (indexDefined)
				diag.report("duplicate directive 'index'", *it);
			else if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					const std::string indexFile = arg->rawContent();
					if (std::find(server.indexFiles().begin(), server.indexFiles().end(), indexFile) != server.indexFiles().end())
					{
						diag.report("duplicate index file '" + indexFile + "'", *arg);
						continue;
					}
					server.indexFiles().push_back(indexFile);
				}
			}
			indexDefined = true;
		}
		else if (name == "error_page")
		{
			if (!errorPageDefined)
				server.errorPages().clear();
			errorPageDefined = true;
			if (checkShape(*it, ARGS_AT_LEAST_TWO, BODY_FORBIDDEN, diag))
			{
				std::string							path = it->args().back().rawContent();
				std::vector<Word>::const_iterator	penultimate = --(--it->args().end());
				int									response_code = 0;
				if (penultimate->rawContent()[0] == '=')
				{
					if (!parseInt(penultimate->rawContent().substr(1), response_code))
						diag.report("invalid response code '" + penultimate->rawContent().substr(1) + "'", *penultimate);
					if (response_code < 100 || response_code > 599)
						diag.report("response code must be between 100 and 599", *penultimate);
				}
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != penultimate; ++arg)
				{
					int code;
					if (!parseInt(arg->rawContent(), code))
					{
						diag.report("invalid response code '" + arg->rawContent() + "'", *arg);
						continue;
					}
					if (code < 100 || code > 599)
					{
						diag.report("response code must be between 100 and 599", *arg);
						continue;
					}
					if (server.errorPages().count(code) > 0)
						diag.report("duplicate error page for code '" + arg->rawContent() + "'", *arg);
					else
						server.errorPages()[code] = std::make_pair((response_code ? response_code : code), path);
				}
			}
		}
		else if (name == "client_max_body_size")
		{
			if (clientMaxBodySizeDefined)
				diag.report("duplicate directive 'client_max_body_size'", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
			{
				size_t size;
				if (!parseSize(it->args().front().rawContent(), size))
					diag.report("invalid size '" + it->args().front().rawContent() + "'", it->args().front());
				else
					server.clientMaxBodySize() = size;
			}
			clientMaxBodySizeDefined = true;
		}
		else if (name == "types")
		{
			if (!typesDefined)
				server.types().clear();
			typesDefined = true;
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				analyseTypes(it->children(), server.types(), diag);
		}
		else if (name == "location")
		{
			bool duplicateLocation = false;
			const std::string locationPath = it->args().front().content();
			for (std::vector<Location>::const_iterator loc = server.locations().begin(); loc != server.locations().end(); ++loc)
			{
				if (loc->path() == locationPath)
				{
					duplicateLocation = true;
					diag.report("duplicate location '" + locationPath + "'", *it);
				}
			}
			if (!duplicateLocation)
				server.locations().push_back(analyseLocation(it->children(), server, locationPath, diag));
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return server;
}

template <typename Type>
Location	Semantic::analyseLocation(const std::list<Directive> &directives, Type &parent, const std::string &path, DiagnosticContext &diag)
{
	Location	location(parent, path);
	bool		rootDefined = false;
	bool		indexDefined = false;
	bool		errorPageDefined = false;
	bool		clientMaxBodySizeDefined = false;
	bool		allowMethodsDefined = false;
	bool		autoindexDefined = false;
	bool		redirectionDefined = false;
	bool		uploadPathDefined = false;
	bool		typesDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "root")
		{
			if (rootDefined)
				diag.report("duplicate 'root' directive", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
				location.root() = it->args().front().rawContent();
			rootDefined = true;
		}
		else if (name == "index")
		{
			if (indexDefined)
				diag.report("duplicate 'index' directive", *it);
			else if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					const std::string indexFile = arg->rawContent();
					if (std::find(location.indexFiles().begin(), location.indexFiles().end(), indexFile) != location.indexFiles().end())
					{
						diag.report("duplicate index file '" + indexFile + "'", *arg);
						continue;
					}
					location.indexFiles().push_back(indexFile);
				}
			}
			indexDefined = true;
		}
		else if (name == "error_page")
		{
			if (!errorPageDefined)
				location.errorPages().clear();
			errorPageDefined = true;
			if (checkShape(*it, ARGS_AT_LEAST_TWO, BODY_FORBIDDEN, diag))
			{
				std::string							path = it->args().back().rawContent();
				std::vector<Word>::const_iterator	penultimate = --(--it->args().end());
				int									response_code = 0;
				if (penultimate->rawContent()[0] == '=')
				{
					if (!parseInt(penultimate->rawContent().substr(1), response_code))
						diag.report("invalid response code '" + penultimate->rawContent().substr(1) + "'", *penultimate);
					if (response_code < 100 || response_code > 599)
						diag.report("response code must be between 100 and 599", *penultimate);
				}
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != penultimate; ++arg)
				{
					int code;
					if (!parseInt(arg->rawContent(), code))
					{
						diag.report("invalid response code '" + arg->rawContent() + "'", *arg);
						continue;
					}
					if (code < 100 || code > 599)
					{
						diag.report("response code must be between 100 and 599", *arg);
						continue;
					}
					if (location.errorPages().count(code) > 0)
						diag.report("duplicate error page for code '" + arg->rawContent() + "'", *arg);
					else
						location.errorPages()[code] = std::make_pair((response_code ? response_code : code), path);
				}
			}
		}
		else if (name == "client_max_body_size")
		{
			if (clientMaxBodySizeDefined)
				diag.report("duplicate 'client_max_body_size' directive", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
			{
				size_t size;
				if (!parseSize(it->args().front().rawContent(), size))
					diag.report("invalid size '" + it->args().front().rawContent() + "'", it->args().front());
				else
					location.clientMaxBodySize() = size;
			}
			clientMaxBodySizeDefined = true;
		}
		else if (name == "allow_methods")
		{
			if (allowMethodsDefined)
				diag.report("duplicate 'allow_methods' directive", *it);
			else if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					const std::string method = arg->rawContent();
					if (std::find(location.allowedMethods().begin(), location.allowedMethods().end(), method) != location.allowedMethods().end())
					{
						diag.report("duplicate allowed method '" + method + "'", *arg);
						continue;
					}
					location.allowedMethods().push_back(method);
				}
			}
			allowMethodsDefined = true;
		}
		else if (name == "autoindex")
		{
			if (autoindexDefined)
				diag.report("duplicate 'autoindex' directive", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
			{
				const std::string value = it->args().front().rawContent();
				if (value == "on")
					location.autoindex() = true;
				else if (value == "off")
					location.autoindex() = false;
				else
					diag.report("invalid value '" + value + "' for 'autoindex' directive", it->args().front());
			}
			autoindexDefined = true;
		}
		else if (name == "return")
		{
			if (redirectionDefined)
				diag.report("duplicate 'return' directive", *it);
			else if (checkShape(*it, ARGS_EXACT_TWO, BODY_FORBIDDEN, diag))
			{
				int code;
				if (!parseInt(it->args().front().rawContent(), code))
					diag.report("invalid response code '" + it->args().front().rawContent() + "'", it->args().front());
				else if (code < 100 || code > 599)
					diag.report("response code must be between 100 and 599", it->args().front());
				else
					location.redirection() = std::make_pair(code, it->args().back().rawContent());
			}
			redirectionDefined = true;
		}
		else if (name == "cgi")
		{
			if (checkShape(*it, ARGS_EXACT_TWO, BODY_FORBIDDEN, diag))
			{
				const std::string extension = it->args().front().rawContent();
				const std::string path = it->args().back().rawContent();
				if (location.cgi().count(extension))
					diag.report("duplicate CGI extension '" + extension + "'", it->args().front());
				else
					location.cgi()[extension] = path;
			}
		}
		else if (name == "upload_path")
		{
			if (uploadPathDefined)
				diag.report("duplicate 'upload_path' directive", *it);
			else if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
				location.uploadPath() = it->args().front().rawContent();
			uploadPathDefined = true;
		}
		else if (name == "types")
		{
			if (!typesDefined)
				location.types().clear();
			typesDefined = true;
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				analyseTypes(it->children(), location.types(), diag);
		}
		else if (name == "location")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_REQUIRED, diag))
			{
				bool duplicateLocation = false;
				const std::string locationPath = it->args().front().content();
				for (std::vector<Location>::const_iterator loc = location.locations().begin(); loc != location.locations().end(); ++loc)
				{
					if (loc->path() == locationPath)
					{
						duplicateLocation = true;
						diag.report("duplicate location '" + locationPath + "'", *it);
					}
				}
				if (!duplicateLocation)
					location.locations().push_back(analyseLocation(it->children(), location, locationPath, diag));
			}
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return location;
}

void	Semantic::analyseTypes(const std::list<Directive> &directives, MimeTypes &types, DiagnosticContext &diag)
{
	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		if (!checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			continue;
		const std::string mime = it->name().rawContent();
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			const std::string ext = arg->content();
			if (types.types().count(ext))
				diag.report("duplicate extension '" + ext + "'", *arg);
			else
				types.types()[ext] = mime;
		}
	}
}
