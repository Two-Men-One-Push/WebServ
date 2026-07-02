#include "Debug.hpp"
#include <list>

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

static void	printLocations(std::ostream &os, const std::vector<Location> &location, size_t indent)
{
	for (std::vector<Location>::const_iterator it = location.begin(); it != location.end(); ++it)
	{
		os << std::string(indent, '\t') << "Location: " << it->path() << std::endl;
		os << std::string(indent + 1, '\t') << "Root: " << it->root() << std::endl;
		os << std::string(indent + 1, '\t') << "Index Files: ";
		for (std::vector<std::string>::const_iterator index_it = it->indexFiles().begin(); index_it != it->indexFiles().end(); ++index_it)
		{
			if (index_it != it->indexFiles().begin())
				os << ", ";
			os << *index_it;
		}
		os << std::endl;
		os << std::string(indent + 1, '\t') << "Error Pages:" << std::endl;
		for (std::map<int, std::pair<int, std::string> >::const_iterator error_it = it->errorPages().begin(); error_it != it->errorPages().end(); ++error_it)
		{
			os << std::string(indent + 2, '\t') << "error code: " << error_it->first << " response code: " << error_it->second.first << " page: " << error_it->second.second << std::endl;
		}
		os << std::string(indent + 1, '\t') << "Client Max Body Size: " << it->clientMaxBodySize() << std::endl;
		os << std::string(indent + 1, '\t') << "Allowed Methods: ";
		for (std::vector<std::string>::const_iterator method_it = it->allowedMethods().begin(); method_it != it->allowedMethods().end(); ++method_it)
		{
			if (method_it != it->allowedMethods().begin())
				os << ", ";
			os << *method_it;
		}
		os << std::endl;
		os << std::string(indent + 1, '\t') << "Autoindex: " << (it->autoindex() ? "on" : "off") << std::endl;
		os << std::string(indent + 1, '\t') << "Redirection: ";
		if (it->redirection().first != 0)
		{
			if (it->redirection().first >= 300 || it->redirection().first <= 399)
				os << "code: " << it->redirection().first << " url: " << it->redirection().second;
			else
				os << "code: " << it->redirection().first << " message: " << it->redirection().second;
		}
		else
			os << "none";
		os << std::endl;
		os << std::string(indent + 1, '\t') << "CGI: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator cgi_it = it->cgi().begin(); cgi_it != it->cgi().end(); ++cgi_it)
		{
			os << std::string(indent + 2, '\t') << "extension: " << cgi_it->first << " interpreter: " << cgi_it->second << std::endl;
		}
		os << std::string(indent + 1, '\t') << "Upload Path: " << it->uploadPath() << std::endl;
		os << std::string(indent + 1, '\t') << "Types:" << std::endl;
		for (std::map<std::string, std::string>::const_iterator type_it = it->types().types().begin(); type_it != it->types().types().end(); ++type_it)
		{
			os << std::string(indent + 2, '\t') << "extension: " << type_it->second << " mimetype: " << type_it->first << std::endl;
		}
		if (!it->locations().empty())
			printLocations(os, it->locations(), indent + 1);
	}
}

void	Debug::printConfig(std::ostream &os, const Config &config)
{
	os << "Config:" << std::endl;
	os << "	Http:" << std::endl;
	os << "		Client Max Body Size: " << config.http().clientMaxBodySize() << std::endl;
	os << "		Error Pages:" << std::endl;
	for (std::map<int, std::pair<int, std::string> >::const_iterator it = config.http().errorPages().begin(); it != config.http().errorPages().end(); ++it)
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
		os << "			extension: " << it->second << " mimetype: "<< it->first << std::endl;
	}
	os << "		Servers:" << std::endl;
	for (std::vector<Server>::const_iterator it = config.http().servers().begin(); it != config.http().servers().end(); ++it)
	{
		os << "			Server:" << std::endl;
		os << "				Listen: ";
		for (std::vector<int>::const_iterator port_it = it->listen().begin(); port_it != it->listen().end(); ++port_it)
		{
			if (port_it != it->listen().begin())
				os << ", ";
			os << *port_it;
		}
		os << std::endl;
		os << "				Server Names: ";
		for (std::vector<std::string>::const_iterator name_it = it->serverNames().begin(); name_it != it->serverNames().end(); ++name_it)
		{
			if (name_it != it->serverNames().begin())
				os << ", ";
			os << *name_it;
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
		for (std::map<int, std::pair<int, std::string> >::const_iterator error_it = it->errorPages().begin(); error_it != it->errorPages().end(); ++error_it)
		{
			os << "					error code: " << error_it->first << " response code: " << error_it->second.first << " page: " << error_it->second.second << std::endl;
		}
		os << "				Client Max Body Size: " << it->clientMaxBodySize() << std::endl;
		os << "				Allowed Methods: ";
		for (std::vector<std::string>::const_iterator method_it = it->allowedMethods().begin(); method_it != it->allowedMethods().end(); ++method_it)
		{
			if (method_it != it->allowedMethods().begin())
				os << ", ";
			os << *method_it;
		}
		os << std::endl;
		os << "				Autoindex: " << (it->autoindex() ? "on" : "off") << std::endl;
		os << "				Redirection: ";
		if (it->redirection().first != 0)
		{
			if (it->redirection().first >= 300 || it->redirection().first <= 399)
				os << "code: " << it->redirection().first << " url: " << it->redirection().second;
			else
				os << "code: " << it->redirection().first << " message: " << it->redirection().second;
		}
		else
			os << "none";
		os << std::endl;
		os << "				CGI:" << std::endl;
		for (std::map<std::string, std::string>::const_iterator cgi_it = it->cgi().begin(); cgi_it != it->cgi().end(); ++cgi_it)
		{
			os << "					extension: " << cgi_it->first << " interpreter: " << cgi_it->second << std::endl;
		}
		os << "				Upload Path: " << it->uploadPath() << std::endl;
		os << "				Types:" << std::endl;
		for (std::map<std::string, std::string>::const_iterator type_it = it->types().types().begin(); type_it != it->types().types().end(); ++type_it)
		{
			os << "					extension: " << type_it->second << " mimetype: " << type_it->first << std::endl;
		}
		printLocations(os, it->locations(), 4);
	}
	os << std::endl;
}