#include "Semantic.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include <sstream>
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
		if (directive.getName().getRawContent().compare("server") == 0)
		{
			if (directive.getArgs().size() > 0)
			{
				throw SemanticInvalidArguments("Server directive cannot have arguments", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			else if (!directive.hasBody())
			{
				throw SemanticBodyNotSpecified("Server directive must have a body", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			Server	server = analyseServer(directive.getChildren());
			config.addServer(server);
		}
		else
		{
			throw SemanticUnknownDirective(directive.getName().getRawContent(), directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
		}
	}
	return (config);
}

Server	Semantic::analyseServer(const std::list<Directive> &directives)
{
	Server	server;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const Directive	&directive = *it;
		if (directive.getName().getRawContent().compare("location") == 0)
		{
			if (directive.getArgs().size() != 1)
			{
				throw SemanticInvalidArguments("Location directive requires exactly one argument", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			else if (!directive.hasBody())
			{
				throw SemanticBodyNotSpecified("Location directive must have a body", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			Location	location = analyseLocation(directive.getChildren());
			server.addLocation(location);
		}
		else
		{
			throw SemanticUnknownDirective(directive.getName().getRawContent(), directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
		}
	}
	return (server);
}

Location	Semantic::analyseLocation(const std::list<Directive> &directives)
{
	Location	location;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		const Directive	&directive = *it;
		if (directive.getName().getRawContent().compare("location") == 0)
		{
			if (directive.getArgs().size() != 1)
			{
				throw SemanticInvalidArguments("Location directive requires exactly one argument", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			else if (!directive.hasBody())
			{
				throw SemanticBodyNotSpecified("Location directive must have a body", directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
			}
			Location	location = analyseLocation(directive.getChildren());
			location.addLocation(location);
		}
		else
		{
			throw SemanticUnknownDirective(directive.getName().getRawContent(), directive.getFilename(), directive.getLineNumber(), directive.getColumnNumber());
		}
	}
	return (location);
}

Semantic::SemanticUnknownDirective::~SemanticUnknownDirective() throw()
{
}

Semantic::SemanticUnknownDirective::SemanticUnknownDirective(const std::string &directive, const std::string &filename, size_t line_number, size_t column_number)
{
	std::stringstream	ss;
	ss << filename << ":" << line_number << ":" << column_number << " Unknown directive '" << directive << "'";
	_message = ss.str();
}

const char	*Semantic::SemanticUnknownDirective::what() const throw()
{
	return (_message.c_str());
}

Semantic::SemanticInvalidArguments::~SemanticInvalidArguments() throw()
{
}

Semantic::SemanticInvalidArguments::SemanticInvalidArguments(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticInvalidArguments::what() const throw()
{
	return _message.c_str();
}

Semantic::SemanticIllegalBody::~SemanticIllegalBody() throw()
{
}

Semantic::SemanticIllegalBody::SemanticIllegalBody(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticIllegalBody::what() const throw()
{
	return _message.c_str();
}

Semantic::SemanticBodyNotSpecified::~SemanticBodyNotSpecified() throw()
{
}

Semantic::SemanticBodyNotSpecified::SemanticBodyNotSpecified(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticBodyNotSpecified::what() const throw()
{
	return _message.c_str();
}