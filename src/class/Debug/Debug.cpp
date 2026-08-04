#include "Debug.hpp"
#include "http/types.hpp"
#include <list>
#include <ostream>

void	Debug::printSegment(std::ostream &os, const Segment &segment)
{
	os << segment.filename() << ":" << segment.lineNumber() << ":" << segment.columnNumber()
	   << "\t\tSegment:[" << segment.rawContent() << " | " << segment.content() << "]" << std::endl;
}

void	Debug::printWord(std::ostream &os, const Word &word)
{
	os << word.filename() << ":" << word.lineNumber() << ":" << word.columnNumber()
	   << "\tContent:[" << word.rawContent() << " | " << word.content() << "]" << std::endl;
	const std::vector<Segment>	&segs = word.segments();
	for (size_t i = 0; i < segs.size(); ++i)
		printSegment(os, segs[i]);
}

void	Debug::printToken(std::ostream &os, const Token &token)
{
	os << token.filename() << ":" << token.lineNumber() << ":" << token.columnNumber()
	   << "\t" << token.typeString() << std::endl;
	if (token.type() == Token::WORD)
		printWord(os, token.word());
}

void	Debug::printTokenStream(std::ostream &os, const TokenStream &token_stream)
{
	const std::vector<Token>	&toks = token_stream.tokens();

	os << "TokenStream:" << std::endl;
	for (size_t i = 0; i < toks.size(); ++i)
		printToken(os, toks[i]);
	os << std::endl;
}

void	Debug::printDirective(std::ostream &os, const Directive &directive, size_t indent)
{
	os << directive.filename() << ":" << directive.lineNumber() << ":" << directive.columnNumber() << "\t";
	for (size_t i = 0; i < indent; ++i)
		os << "\t";
	os << "Directive: [" << directive.name().rawContent() << "] [" << directive.name().content() << "]" << std::endl;
	for (size_t i = 0; i < directive.args().size(); ++i)
	{
		const Word	&arg = directive.args()[i];
		os << arg.filename() << ":" << arg.lineNumber() << ":" << arg.columnNumber() << "\t";
		for (size_t j = 0; j < indent + 1; ++j)
			os << "\t";
		os << "Arg: [" << arg.rawContent() << "] [" << arg.content() << "]" << std::endl;
	}
	if (!directive.children().empty())
	{
		for (size_t i = 0; i < indent + 2; ++i)
			os << "\t";
		os << "Children:" << std::endl;
	}
	for (std::list<Directive>::const_iterator it = directive.children().begin(); it != directive.children().end(); ++it)
		printDirective(os, *it, indent + 1);
}

void	Debug::printAST(std::ostream &os, const AST &ast)
{
	os << "AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
		printDirective(os, *it, 0);
	os << std::endl;
}

void	Debug::printPreprocessedAST(std::ostream &os, const AST &ast)
{
	os << "Preprocessed AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
		printDirective(os, *it, 0);
	os << std::endl;
}

void	Debug::printLocation(std::ostream &os, const Location &location, size_t indent)
{
	os << std::string(indent, '\t') << "Location: " << location.path() << std::endl;
	os << std::string(indent + 1, '\t') << "Root: " << location.root() << std::endl;
	os << std::string(indent + 1, '\t') << "Index Files: ";
	for (std::vector<std::string>::const_iterator index_it = location.indexFiles().begin(); index_it != location.indexFiles().end(); ++index_it)
	{
		if (index_it != location.indexFiles().begin())
			os << ", ";
		os << *index_it;
	}
	os << std::endl;
	os << std::string(indent + 1, '\t') << "Error Pages:" << std::endl;
	for (std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >::const_iterator error_it = location.errorPages().begin(); error_it != location.errorPages().end(); ++error_it)
	{
		os << std::string(indent + 2, '\t') << "error code: " << error_it->first << " response code: " << error_it->second.first << " page: " << error_it->second.second << std::endl;
	}
	os << std::string(indent + 1, '\t') << "Client Max Body Size: " << location.maxBodySize() << std::endl;
	os << std::string(indent + 1, '\t') << "Allowed Methods: ";
	for (std::vector<HttpMethod>::const_iterator method_it = location.allowedMethods().begin(); method_it != location.allowedMethods().end(); ++method_it)
	{
		if (method_it != location.allowedMethods().begin())
			os << ", ";
		os << httpMethodString(*method_it);
	}
	os << std::endl;
	os << std::string(indent + 1, '\t') << "Autoindex: " << (location.autoindex() ? "on" : "off") << std::endl;
	os << std::string(indent + 1, '\t') << "Redirection: " << location.redirection() << std::endl;
	os << std::string(indent + 1, '\t') << "CGI: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator cgi_it = location.cgi().begin(); cgi_it != location.cgi().end(); ++cgi_it)
	{
		os << std::string(indent + 2, '\t') << "extension: " << cgi_it->first << " interpreter: " << cgi_it->second << std::endl;
	}
	os << std::string(indent + 1, '\t') << "Editable: " << location.editable() << std::endl;
	os << std::string(indent + 1, '\t') << "Types: ..." << std::endl;
	// for (std::map<std::string, std::string>::const_iterator type_it = location.types().types().begin(); type_it != location.types().types().end(); ++type_it)
	// {
	// 	os << std::string(indent + 2, '\t') << "extension: " << type_it->first << " mimetype: " << type_it->second << std::endl;
	// }
}

void	Debug::printConfig(std::ostream &os, const Config &config)
{
	os << "Config:" << std::endl;
	os << "	Http:" << std::endl;
	os << "		Client Max Body Size: " << config.http().maxBodySize() << std::endl;
	os << "		Error Pages:" << std::endl;
	for (std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >::const_iterator it = config.http().errorPages().begin(); it != config.http().errorPages().end(); ++it)
	{
		os << "			error code: " << it->first << " response code: " << it->second.first << " page: " << it->second.second << std::endl;
	}
	os << "		CGI:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = config.http().cgi().begin(); it != config.http().cgi().end(); ++it)
	{
		os << "			extension: " << it->first << " interpreter: " << it->second << std::endl;
	}
	os << "		Types:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = config.http().types().types().begin(); it != config.http().types().types().end(); ++it)
	{
		os << "			extension: " << it->first << " mimetype: "<< it->second << std::endl;
	}
	os << "		Servers:" << std::endl;
	for (std::vector<Server>::const_iterator it = config.http().servers().begin(); it != config.http().servers().end(); ++it)
	{
		os << "			Server:" << std::endl;
		os << "				Listen: ";
		for (std::vector<std::pair<std::string, int> >::const_iterator listen_it = it->listen().begin(); listen_it != it->listen().end(); ++listen_it)
		{
			if (listen_it != it->listen().begin())
				os << ", ";
			os << listen_it->first << ":" << listen_it->second;
		}
		os << std::endl;
		os << "				Root: " << it->root() << std::endl;
		os << "				Index Files: ";
		for (std::vector<std::string>::const_iterator index_it = it->indexFiles().begin(); index_it != it->indexFiles().end(); ++index_it)
		{
			if (index_it != it->indexFiles().begin())
				os << ", ";
			os << *index_it;
		}
		os << std::endl;
		os << "				Error Pages:" << std::endl;
		for (std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >::const_iterator error_it = it->errorPages().begin(); error_it != it->errorPages().end(); ++error_it)
		{
			os << "					error code: " << error_it->first << " response code: " << error_it->second.first << " page: " << error_it->second.second << std::endl;
		}
		os << "				Client Max Body Size: " << it->maxBodySize() << std::endl;
		os << "				Allowed Methods: ";
		for (std::vector<HttpMethod>::const_iterator method_it = it->allowedMethods().begin(); method_it != it->allowedMethods().end(); ++method_it)
		{
			if (method_it != it->allowedMethods().begin())
				os << ", ";
			os << httpMethodString(*method_it);
		}
		os << std::endl;
		os << "				Autoindex: " << (it->autoindex() ? "on" : "off") << std::endl;
		os << "				Redirection:" << it->redirection() << std::endl;
		os << "				CGI:" << std::endl;
		for (std::map<std::string, std::string>::const_iterator cgi_it = it->cgi().begin(); cgi_it != it->cgi().end(); ++cgi_it)
		{
			os << "					extension: " << cgi_it->first << " interpreter: " << cgi_it->second << std::endl;
		}
		os << "				Editable: " << it->editable() << std::endl;
		os << "				Types:" << std::endl;
		for (std::map<std::string, std::string>::const_iterator type_it = it->types().types().begin(); type_it != it->types().types().end(); ++type_it)
		{
			os << "					extension: " << type_it->first << " mimetype: " << type_it->second << std::endl;
		}
		for (std::vector<Location>::const_iterator location_it = it->locations().begin(); location_it != it->locations().end(); ++location_it)
		{
			printLocation(os, *location_it, 4);
		}
	}
	os << std::endl;
}

void	Debug::printURL(std::ostream &os, const URL &url)
{
	os << "URL:" << std::endl;
	os << "	Format: " << url.formatStr() << std::endl;
	os << "	Raw URL: " << url.raw() << std::endl;
	os << "	Scheme: " << url.scheme() << std::endl;
	os << "	User: " << url.user() << std::endl;
	os << "	Host: " << url.host() << std::endl;
	os << "	Port: " << url.port() << std::endl;
	os << "	Raw URL Segments:		";
	for (std::vector<std::string>::const_iterator it = url.rawSegments().begin(); it != url.rawSegments().end(); ++it)
	{
		if (it != url.rawSegments().begin())
			os << ", ";
		os << *it;
	}
	os << std::endl;
	os << "	URL Segments:			";
	for (std::vector<std::string>::const_iterator it = url.segments().begin(); it != url.segments().end(); ++it)
	{
		if (it != url.segments().begin())
			os << ", ";
		os << *it;
	}
	os << std::endl;
	os << "	Normalized URL Segments:	";
	for (std::vector<std::string>::const_iterator it = url.normalizedSegments().begin(); it != url.normalizedSegments().end(); ++it)
	{
		if (it != url.normalizedSegments().begin())
			os << ", ";
		os << *it;
	}
	os << std::endl;
	os << "	Query String: " << url.queryString() << std::endl;
	os << "	Query Parameters: ";
	for (std::map<std::string, std::string>::const_iterator it = url.query().begin(); it != url.query().end(); ++it)
	{
		if (it != url.query().begin())
			os << ", ";
		os << it->first << "=" << it->second;
	}
	os << std::endl;
	os << "	Fragment String: " << url.fragmentString() << std::endl;
	os << "	Fragment: " << url.fragment() << std::endl;
}

void	Debug::printRessource(std::ostream &os, const Ressource &ressource)
{
	os << "Ressource:" << std::endl;
	os << "	Type: " << ressource.typeStr() << std::endl;
	os << "	Root: " << ressource.root() << std::endl;
	os << "	Path: " << ressource.path() << std::endl;
	os << "	MIME Type: " << ressource.mimeType() << std::endl;
	os << "	Response Code: " << ressource.responseCode() << std::endl;
	os << "	CGI Interpreter: " << ressource.cgiInterpreter() << std::endl;
	os << "	Path Info: " << ressource.pathInfo() << std::endl;
	os << " Allowed Methods: ";
	for (std::vector<HttpMethod>::const_iterator it = ressource.allowedMethods().begin(); it != ressource.allowedMethods().end(); ++it)
		os << httpMethodString(*it) << ", ";
	os << std::endl;
	os << " Location: " << std::endl;
	Debug::printLocation(os, ressource.location(), 1);
}
