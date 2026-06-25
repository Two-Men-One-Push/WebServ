#include "Semantic.hpp"
#include "Utils.hpp"
#include "Word.hpp"

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
	if (body == BODY_FORBIDDEN && d.hasBody())
		diag.report("'" + n + "' directive cannot have a body", d.blockErrorInfo());
	else if (body == BODY_REQUIRED && !d.hasBody())
	{
		diag.report("'" + n + "' directive requires a body", d);
		canProcess = false;
	}
	return canProcess;
}

Config	Semantic::analyseAST(const AST &ast)
{
	Config	config;

	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "http")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, config.diag()))
				config.http() = analyseHttp(it->children(), config.diag());
		}
		else
			config.diag().report("unknown directive '" + name + "'", *it);
	}
	return config;
}

Http	Semantic::analyseHttp(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	Http	http;
	bool	typesDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "server")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				http.servers().push_back(analyseServer(it->children(), http, diag));
		}
		else if (name == "types")
		{
			if (typesDefined)
				diag.report("duplicate directive 'types'", *it);
			typesDefined = true;
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				http.mimetype() = analyseTypes(it->children(), diag);
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return http;
}

Server	Semantic::analyseServer(const std::list<Directive> &directives, Http &http, DiagnosticContext &diag)
{
	Server	server(http);
	bool	typesDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "listen")
		{
			if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
				{
					int port;
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
					server.listen().push_back(port);
				}
			}
		}
		else if (name == "server_name")
		{
			if (checkShape(*it, ARGS_REQUIRED, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
					server.serverNames().push_back(arg->rawContent());
			}
		}
		else if (name == "root")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_FORBIDDEN, diag))
				server.root() = it->args().front().rawContent();
		}
		else if (name == "index")
		{
			if (checkShape(*it, ARGS_AT_LEAST_ONE, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
					server.indexFiles().push_back(arg->rawContent());
			}
		}
		else if (name == "error_page")
		{
			if (checkShape(*it, ARGS_AT_LEAST_TWO, BODY_FORBIDDEN, diag))
			{
				std::string							path = it->args().back().rawContent();
				std::vector<Word>::const_iterator	penultimate = --(--it->args().end());
				int									response_code;
				bool								has_response_code = false;
				if (!penultimate->rawContent().empty() && penultimate->rawContent()[0] == '=')
				{
					has_response_code = true;
					if (!parseInt(penultimate->rawContent().substr(1), response_code))
					{
						diag.report("invalid response code '" + penultimate->rawContent().substr(1) + "'", *penultimate);
						continue;
					}
					if (response_code < 100 || response_code > 599)
					{
						diag.report("response code must be between 100 and 599", *penultimate);
						continue;
					}
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
					if (has_response_code)
						server.errorPages()[code] = std::make_pair(response_code, path);
					else
						server.errorPages()[code] = std::make_pair(code, path);
				}
			}
		}
		else if (name == "types")
		{
			if (typesDefined)
			{
				diag.report("duplicate directive 'types'", *it);
				continue;
			}
			typesDefined = true;
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				server.mimetype() = analyseTypes(it->children(), diag);
		}
		else if (name == "location")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_REQUIRED, diag))
				server.locations().push_back(analyseLocation(it->children(), server, it->args().front().content(), diag));
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
	bool		typesDefined = false;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "types")
		{
			if (typesDefined)
			{
				diag.report("duplicate directive 'types'", *it);
				continue;
			}
			typesDefined = true;
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				location.mimetype() = analyseTypes(it->children(), diag);
		}
		else if (name == "location")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_REQUIRED, diag))
				location.locations().push_back(analyseLocation(it->children(), location, it->args().front().content(), diag));
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return location;
}

MimeTypes	Semantic::analyseTypes(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	MimeTypes	types;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		if (!checkShape(*it, ARGS_REQUIRED, BODY_FORBIDDEN, diag))
			continue;
		const std::string mime = it->name().rawContent();
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			const std::string ext = arg->content();
			if (types.mimetypes().count(ext))
				diag.report("duplicate extension '" + ext + "'", *arg);
			else
				types.mimetypes()[ext] = mime;
		}
	}
	return types;
}
