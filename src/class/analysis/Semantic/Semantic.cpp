#include "Semantic.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include "Word.hpp"
#include <iostream>
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
		if (directive.getName().getRawContent().compare("http") == 0)
		{
			bool	canProcess = true;
			if (directive.getArgs().size() > 0)
			{
				for (std::vector<Word>::const_iterator arg_it = directive.getArgs().begin(); arg_it != directive.getArgs().end(); ++arg_it)
				{
					std::cerr << SemanticInvalidArguments("Http directive cannot have arguments", *arg_it).what() << std::endl;
				}
			}
			if (!directive.hasBody())
			{
				std::cerr << SemanticBodyNotSpecified("Http directive must have a body", directive).what() << std::endl;
				canProcess = false;
			}
			if (config.isHttpSet())
			{
				std::cerr << SemanticInvalidDirective("Http directive already specified", directive).what() << std::endl;
				canProcess = false;
			}
			if (canProcess)
			{
				config.setHttp(analyseHttp(directive.getChildren()));
			}
		}
		else
		{
			std::cerr << SemanticUnknownDirective(directive.getName().getRawContent(), directive).what() << std::endl;
		}
	}
	return (config);
}

Http	Semantic::analyseHttp(const std::list<Directive> &directives)
{
	Http	http;

	for (std::list<Directive>::const_iterator directive_it = directives.begin(); directive_it != directives.end(); ++directive_it)
	{
		if (directive_it->getName().getRawContent().compare("server") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() > 0)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					std::cerr << SemanticInvalidArguments("Server directive cannot have arguments", *arg_it).what() << std::endl;
				}
			}
			else if (!directive_it->hasBody())
			{
				std::cerr << SemanticBodyNotSpecified("Server directive must have a body", *directive_it).what() << std::endl;
				canProcess = false;
			}
			if (canProcess)
			{
				http.addServer(analyseServer(directive_it->getChildren()));
			}
		}
		else
		{
			std::cerr << SemanticUnknownDirective(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
		}
	}
	return (http);
}

Server	Semantic::analyseServer(const std::list<Directive> &directives)
{
	Server	server;

	for (std::list<Directive>::const_iterator directive_it = directives.begin(); directive_it != directives.end(); ++directive_it)
	{
		if (directive_it->getName().getRawContent().compare("listen") == 0)
		{
			bool	canProcess = true;

			if (directive_it->getArgs().size() < 1)
			{
				std::cerr << SemanticInvalidArguments("Listen directive requires at least one argument", *directive_it).what() << std::endl;
				canProcess = false;
			}
			if (directive_it->hasBody())
			{
				std::cerr << SemanticIllegalBody("Listen directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
			}
			if (canProcess)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					server.addListen(arg_it->getContent());
				}
			}
		}
		else if (directive_it->getName().getRawContent().compare("location") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() != 1)
			{
				std::cerr << SemanticInvalidArguments("Location directive requires exactly one argument", *directive_it).what() << std::endl;
			}
			if (!directive_it->hasBody())
			{
				std::cerr << SemanticBodyNotSpecified("Location directive must have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				canProcess = false;
			}
			if (canProcess)
			{
				server.addLocation(analyseLocation(directive_it->getChildren()));
			}
		}
		else
		{
			std::cerr << SemanticUnknownDirective(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
		}
	}
	if (server.getListen().empty())
	{
		server.addListen("80");
	}
	return (server);
}

Location	Semantic::analyseLocation(const std::list<Directive> &directives)
{
	Location	location;

	for (std::list<Directive>::const_iterator directive_it = directives.begin(); directive_it != directives.end(); ++directive_it)
	{
		if (directive_it->getName().getRawContent().compare("location") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() != 1)
			{
				std::cerr << SemanticInvalidArguments("Location directive requires exactly one argument", *directive_it).what() << std::endl;
			}
			if (!directive_it->hasBody())
			{
				std::cerr << SemanticBodyNotSpecified("Location directive must have a body", *directive_it).what() << std::endl;
				canProcess = false;
			}
			if (canProcess)
			{
				location.addLocation(analyseLocation(directive_it->getChildren()));
			}
		}
		else
		{
			std::cerr << SemanticUnknownDirective(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
		}
	}
	return (location);
}

MimeType	Semantic::analyseMimeType(const std::list<Directive> &directives)
{
	MimeType	mimeType;

	for (std::list<Directive>::const_iterator directive_it = directives.begin(); directive_it != directives.end(); ++directive_it)
	{
		bool	canProcess = true;
		if (directive_it->getArgs().size() < 1)
		{
			std::cerr << SemanticInvalidArguments("MimeType directive requires at least one arguments", *directive_it).what() << std::endl;
			canProcess = false;
		}
		if (directive_it->hasBody())
		{
			std::cerr << SemanticIllegalBody("MimeType directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
		}
		if (canProcess)
		{
			for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
			{
				if (mimeType.getMimeTypes().find(arg_it->getContent()) != mimeType.getMimeTypes().end())
				{
					std::cerr << SemanticInvalidArguments("MimeType directive for extension '" + arg_it->getContent() + "' already specified", *arg_it).what() << std::endl;
				}
				else
				{
					mimeType.addMimeType(arg_it->getContent(), arg_it->getContent());
				}
			}
		}
	}
	return (mimeType);
}

Semantic::SemanticUnknownDirective::~SemanticUnknownDirective() throw()
{
}

Semantic::SemanticUnknownDirective::SemanticUnknownDirective(const std::string &directive, const ErrorInfo &error_info) : _message()
{
	std::stringstream	ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " Unknown directive '" << directive << "'";
	_message = ss.str();
}

const char	*Semantic::SemanticUnknownDirective::what() const throw()
{
	return (_message.c_str());
}

Semantic::SemanticInvalidDirective::~SemanticInvalidDirective() throw()
{
}

Semantic::SemanticInvalidDirective::SemanticInvalidDirective(const std::string &description, const ErrorInfo &error_info) : _message()
{
	std::stringstream	ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticInvalidDirective::what() const throw()
{
	return (_message.c_str());
}

Semantic::SemanticInvalidArguments::~SemanticInvalidArguments() throw()
{
}

Semantic::SemanticInvalidArguments::SemanticInvalidArguments(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticInvalidArguments::what() const throw()
{
	return _message.c_str();
}

Semantic::SemanticIllegalBody::~SemanticIllegalBody() throw()
{
}

Semantic::SemanticIllegalBody::SemanticIllegalBody(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticIllegalBody::what() const throw()
{
	return _message.c_str();
}

Semantic::SemanticBodyNotSpecified::~SemanticBodyNotSpecified() throw()
{
}

Semantic::SemanticBodyNotSpecified::SemanticBodyNotSpecified(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Semantic::SemanticBodyNotSpecified::what() const throw()
{
	return _message.c_str();
}