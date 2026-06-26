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

void	Semantic::parseClientMaxBodySize(std::list<Directive>::const_iterator it, size_t &size, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
	{
		size_t value;
		if (!parseSize(it->args().front().rawContent(), value))
			diag.report("invalid size '" + it->args().front().rawContent() + "'", it->args().front());
		else
			size = value;
	}
}

void	Semantic::parseErrorPages(std::list<Directive>::const_iterator it, std::map<int, std::pair<int, std::string> > &error_pages, DiagnosticContext &diag)
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
			error_pages[code] = std::make_pair(response_code, path);
		}
	}
}

void	Semantic::parseCGI(std::list<Directive>::const_iterator it, std::map<std::string, std::string> &cgi, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_AT_LEAST_TWO, BODY_FORBIDDEN, diag))
	{
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != --(it->args().end()); ++arg)
			cgi[arg->rawContent()] = it->args().back().rawContent();
	}
}

void	Semantic::parseListen(std::list<Directive>::const_iterator it, std::vector<int> &listen, Http &http, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
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
				listen.push_back(port);
		}
	}
}

void	Semantic::parseServerNames(std::list<Directive>::const_iterator it, std::vector<std::string> &server_names, Http &http, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
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
				server_names.push_back(serverName);
		}
	}
}

void	Semantic::parseRoot(std::list<Directive>::const_iterator it, std::string &root, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
		root = it->args().front().rawContent();
}

void	Semantic::parseIndex(std::list<Directive>::const_iterator it, std::vector<std::string> &index_files, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
	{
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			const std::string indexFile = arg->rawContent();
			if (std::find(index_files.begin(), index_files.end(), indexFile) != index_files.end())
			{
				diag.report("duplicate index file '" + indexFile + "'", *arg);
				continue;
			}
			index_files.push_back(indexFile);
		}
	}
}

void	Semantic::parseAllowMethods(std::list<Directive>::const_iterator it, std::vector<std::string> &allowed_methods, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
	{
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			const std::string method = arg->rawContent();
			if (std::find(allowed_methods.begin(), allowed_methods.end(), method) != allowed_methods.end())
			{
				diag.report("duplicate allowed method '" + method + "'", *arg);
				continue;
			}
			allowed_methods.push_back(method);
		}
	}
}

void	Semantic::parseAutoindex(std::list<Directive>::const_iterator it, bool &autoindex, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
	{
		const std::string value = it->args().front().rawContent();
		if (value == "on")
			autoindex = true;
		else if (value == "off")
			autoindex = false;
		else
			diag.report("invalid value '" + value + "' for 'autoindex' directive", it->args().front());
	}
}

void	Semantic::parseRedirection(std::list<Directive>::const_iterator it, std::pair<int, std::string> &redirection, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_EXACT_TWO, BODY_FORBIDDEN, diag))
	{
		int code;
		if (!parseInt(it->args().front().rawContent(), code))
			diag.report("invalid response code '" + it->args().front().rawContent() + "'", it->args().front());
		else if (code < 100 || code > 599)
			diag.report("response code must be between 100 and 599", it->args().front());
		else
			redirection = std::make_pair(code, it->args().back().rawContent());
	}
}

void	Semantic::parseUploadPath(std::list<Directive>::const_iterator it, std::string &upload_path, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
		upload_path = it->args().front().rawContent();
}

void	Semantic::parseTypes(std::list<Directive>::const_iterator it, MimeTypes &types, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
		analyseTypes(it->children(), types, diag);
}

void	Semantic::parseHttp(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
		http = analyseHttp(it->children(), diag);
}

void	Semantic::parseServer(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag)
{
	if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
		http.servers().push_back(analyseServer(it->children(), http, diag));
}

template <typename Type>
void	Semantic::parseLocation(std::list<Directive>::const_iterator it, std::vector<Location> &locations, Type &parent, DiagnosticContext &diag)
{
	bool duplicateLocation = false;
	const std::string locationPath = it->args().front().content();
	for (std::vector<Location>::const_iterator loc = locations.begin(); loc != locations.end(); ++loc)
	{
		if (loc->path() == locationPath)
		{
			duplicateLocation = true;
			diag.report("duplicate location '" + locationPath + "'", *it);
		}
	}
	if (!duplicateLocation)
		locations.push_back(analyseLocation(it->children(), parent, locationPath, diag));
}

Config	Semantic::analyseAST(const AST &ast, DiagnosticContext &diag)
{
	Config	config;

	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "http")
			parseHttp(it, config.http(), diag);
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return config;
}

Http	Semantic::analyseHttp(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	Http	http;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "client_max_body_size")
			parseClientMaxBodySize(it, http.clientMaxBodySize(), diag);
		else if (name == "error_page")
			parseErrorPages(it, http.errorPages(), diag);
		else if (name == "cgi")
			parseCGI(it, http.cgi(), diag);
		else if (name == "types")
			parseTypes(it, http.types(), diag);
		else if (name == "server")
			parseServer(it, http, diag);
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return http;
}

Server	Semantic::analyseServer(const std::list<Directive> &directives, Http &http, DiagnosticContext &diag)
{
	Server	server(http);

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "listen")
			parseListen(it, server.listen(), http, diag);
		else if (name == "server_name")
			parseServerNames(it, server.serverNames(), http, diag);
		else if (name == "root")
			parseRoot(it, server.root(), diag);
		else if (name == "index")
			parseIndex(it, server.indexFiles(), diag);
		else if (name == "error_page")
			parseErrorPages(it, server.errorPages(), diag);
		else if (name == "client_max_body_size")
			parseClientMaxBodySize(it, server.clientMaxBodySize(), diag);
		else if (name == "allow_methods")
			parseAllowMethods(it, server.allowedMethods(), diag);
		else if (name == "autoindex")
			parseAutoindex(it, server.autoindex(), diag);
		else if (name == "redirect")
			parseRedirection(it, server.redirection(), diag);
		else if (name == "cgi")
			parseCGI(it, server.cgi(), diag);
		else if (name == "upload_path")
			parseUploadPath(it, server.uploadPath(), diag);
		else if (name == "types")
			parseTypes(it, server.types(), diag);
		else if (name == "location")
			parseLocation(it, server.locations(), server, diag);
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return server;
}

template <typename Type>
Location	Semantic::analyseLocation(const std::list<Directive> &directives, Type &parent, const std::string &path, DiagnosticContext &diag)
{
	Location	location(parent, path);

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "root")
			parseRoot(it, location.root(), diag);
		else if (name == "index")
			parseIndex(it, location.indexFiles(), diag);
		else if (name == "error_page")
			parseErrorPages(it, location.errorPages(), diag);
		else if (name == "client_max_body_size")
			parseClientMaxBodySize(it, location.clientMaxBodySize(), diag);
		else if (name == "allow_methods")
			parseAllowMethods(it, location.allowedMethods(), diag);
		else if (name == "autoindex")
			parseAutoindex(it, location.autoindex(), diag);
		else if (name == "redirect")
			parseRedirection(it, location.redirection(), diag);
		else if (name == "cgi")
			parseCGI(it, location.cgi(), diag);
		else if (name == "upload_path")
			parseUploadPath(it, location.uploadPath(), diag);
		else if (name == "types")
			parseTypes(it, location.types(), diag);
		else if (name == "location")
			parseLocation(it, location.locations(), location, diag);
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
			types.types()[arg->content()] = mime;
	}
}
