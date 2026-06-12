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
		this->
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
					std::cerr << SemanticError("Server directive cannot have arguments", *arg_it).what() << std::endl;
					http.setError(true);
				}
			}
			else if (!directive_it->hasBody())
			{
				std::cerr << SemanticError("Server directive must have a body", *directive_it).what() << std::endl;
				canProcess = false;
				http.setError(true);
			}
			if (canProcess)
			{
				http.addServer(analyseServer(directive_it->getChildren()));
				if (http.getServers().back().hasError())
				{
					http.setError(true);
				}
			}
		}
		else if (directive_it->getName().getRawContent().compare("mimetype") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() > 0)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					std::cerr << SemanticError("MimeType directive cannot have arguments", *arg_it).what() << std::endl;
					http.setError(true);
				}
			}
			if (directive_it->hasBody())
			{
				std::cerr << SemanticError("MimeType directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				http.setError(true);
			}
			if (canProcess)
			{
				http.setMimeType(analyseMimeType(directive_it->getChildren()));
				if (http.getMimeType().hasError())
				{
					http.setError(true);
				}
			}
		}
		else
		{
			std::cerr << SemanticError(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
			http.setError(true);
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
				std::cerr << SemanticError("Listen directive requires at least one argument", *directive_it).what() << std::endl;
				canProcess = false;
				server.setError(true);
			}
			if (directive_it->hasBody())
			{
				std::cerr << SemanticError("Listen directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				server.setError(true);
			}
			if (canProcess)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					//check if the port is a valid number between 1 and 65535
					server.addListen(arg_it->getContent());
				}
			}
		}
		else if (directive_it->getName().getRawContent().compare("location") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() != 1)
			{
				std::cerr << SemanticError("Location directive requires exactly one argument", *directive_it).what() << std::endl;
				server.setError(true);
			}
			if (!directive_it->hasBody())
			{
				std::cerr << SemanticError("Location directive must have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				canProcess = false;
				server.setError(true);
			}
			if (canProcess)
			{
				server.addLocation(analyseLocation(directive_it->getChildren()));
				if (server.getLocations().back().hasError())
				{
					server.setError(true);
				}
			}
		}
		else if (directive_it->getName().getRawContent().compare("mimetype") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() > 0)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					std::cerr << SemanticError("MimeType directive cannot have arguments", *arg_it).what() << std::endl;
					server.setError(true);
				}
			}
			if (directive_it->hasBody())
			{
				std::cerr << SemanticError("MimeType directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				server.setError(true);
			}
			if (canProcess)
			{
				server.setMimeType(analyseMimeType(directive_it->getChildren()));
				if (server.getMimeType().hasError())
				{
					server.setError(true);
				}
			}
		}
		else
		{
			std::cerr << SemanticError(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
			server.setError(true);
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
				std::cerr << SemanticError("Location directive requires exactly one argument", *directive_it).what() << std::endl;
				location.setError(true);
			}
			if (!directive_it->hasBody())
			{
				std::cerr << SemanticBodyNotSpecified("Location directive must have a body", *directive_it).what() << std::endl;
				canProcess = false;
				location.setError(true);
			}
			if (canProcess)
			{
				location.addLocation(analyseLocation(directive_it->getChildren()));
				if (location.getLocations().back().hasError())
				{
					location.setError(true);
				}
			}
		}
		else if (directive_it->getName().getRawContent().compare("mimetype") == 0)
		{
			bool	canProcess = true;
			if (directive_it->getArgs().size() > 0)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					std::cerr << SemanticInvalidArguments("MimeType directive cannot have arguments", *arg_it).what() << std::endl;
					location.setError(true);
				}
			}
			if (directive_it->hasBody())
			{
				std::cerr << SemanticIllegalBody("MimeType directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
				location.setError(true);
			}
			if (canProcess)
			{
				location.setMimeType(analyseMimeType(directive_it->getChildren()));
				if (location.getMimeType().hasError())
				{
					location.setError(true);
				}
			}
		}
		else
		{
			std::cerr << SemanticUnknownDirective(directive_it->getName().getRawContent(), *directive_it).what() << std::endl;
			location.setError(true);
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
			std::cerr << SemanticInvalidArguments("MimeType requires at least one arguments", *directive_it).what() << std::endl;
			canProcess = false;
			mimeType.setError(true);
		}
		if (directive_it->hasBody())
		{
			std::cerr << SemanticIllegalBody("MimeType cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
			mimeType.setError(true);
		}
		if (canProcess)
		{
			for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
			{
				if (mimeType.getMimeTypes().find(arg_it->getContent()) != mimeType.getMimeTypes().end())
				{
					std::cerr << SemanticInvalidArguments("MimeType for extension '" + arg_it->getContent() + "' already specified", *arg_it).what() << std::endl;
					mimeType.setError(true);
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

void	Semantic::analyseDirective(const Directive &directive, Config &config)
{
	
}

Semantic::SemanticError::~SemanticError() throw()
{
}

const char	*Semantic::SemanticError::what() const throw()
{
	return "Semantic analyzer found errors in the configuration";
}