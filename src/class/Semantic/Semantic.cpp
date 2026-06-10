#include "Semantic.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include <vector>

Semantic::Semantic()
{
}

Semantic::~Semantic()
{
}

Config	Semantic::analyseAST(const AST &ast)
{
	Config	config;

	for (std::list<Directive>::const_iterator it = ast.getDirectives().begin(); it != ast.getDirectives().end(); ++it)
	{
		const Directive	&directive = *it;
		(void)directive;
	}
	return (config);
}

Server	Semantic::analyseServer(const std::list<Directive> &directives)
{
	Server	server;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const Directive	&directive = *it;
		(void)directive;
	}
	return (server);
}

Location	Semantic::analyseLocation(const std::list<Directive> &directives)
{
	Location	location;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const Directive	&directive = *it;
		(void)directive;
	}
	return (location);
}