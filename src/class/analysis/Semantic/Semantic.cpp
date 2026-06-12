#include "Semantic.hpp"
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
	else if (args == ARGS_REQUIRED && d.args().empty())
	{
		diag.report("'" + n + "' directive requires at least one argument", d);
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

Config	Semantic::analyseAST(const AST &ast, DiagnosticContext &diag)
{
	Config config;

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
	Http http;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "server")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				http.servers().push_back(analyseServer(it->children(), diag));
		}
		else if (name == "mimetype")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				http.mimetype() = analyseMimeType(it->children(), diag);
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return http;
}

Server	Semantic::analyseServer(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	Server server;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "listen")
		{
			if (checkShape(*it, ARGS_REQUIRED, BODY_FORBIDDEN, diag))
			{
				for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
					server.listen().push_back(arg->content());
			}
		}
		else if (name == "location")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_REQUIRED, diag))
				server.locations().push_back(analyseLocation(it->children(), diag));
		}
		else if (name == "mimetype")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				server.mimetype() = analyseMimeType(it->children(), diag);
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	if (server.listen().empty())
		server.listen().push_back("80");
	return server;
}

Location	Semantic::analyseLocation(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	Location location;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const std::string name = it->name().rawContent();
		if (name == "location")
		{
			if (checkShape(*it, ARGS_EXACT_ONE, BODY_REQUIRED, diag))
				location.locations().push_back(analyseLocation(it->children(), diag));
		}
		else if (name == "mimetype")
		{
			if (checkShape(*it, ARGS_FORBIDDEN, BODY_REQUIRED, diag))
				location.mimetype() = analyseMimeType(it->children(), diag);
		}
		else
			diag.report("unknown directive '" + name + "'", *it);
	}
	return location;
}

MimeType	Semantic::analyseMimeType(const std::list<Directive> &directives, DiagnosticContext &diag)
{
	MimeType mimeType;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		if (!checkShape(*it, ARGS_REQUIRED, BODY_FORBIDDEN, diag))
			continue;
		const std::string mime = it->name().rawContent();
		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			const std::string ext = arg->content();
			if (mimeType.mimetypes().count(ext))
				diag.report("duplicate extension '" + ext + "'", *arg);
			else
				mimeType.mimetypes()[ext] = mime;
		}
	}
	return mimeType;
}
