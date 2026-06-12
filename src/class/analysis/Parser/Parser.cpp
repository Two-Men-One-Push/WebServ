#include "Parser.hpp"
#include "Directive.hpp"
#include "Token.hpp"
#include <sstream>
#include <list>

Parser::Parser()
{
}

Parser::~Parser()
{
}

AST	Parser::parse(const TokenStream &token_stream)
{
	AST							ast;
	TokenStream::const_iterator	it = token_stream.begin();
	TokenStream::const_iterator	end = token_stream.end();

	ast.filename() = token_stream.filename();
	parseListDirective(it, end, ast.directives());
	if (it->type() != Token::_EOF)
		throw ParserUnexpectedToken("Unexpected token, expected directive or EOF", *it);
	return ast;
}

Directive	Parser::parseDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end)
{
	Directive	directive(it->word(), it->filename(), it->lineNumber(), it->columnNumber());
	++it;

	while (it->type() != Token::_EOF)
	{
		switch (it->type())
		{
			case Token::WORD:
				directive.addArg(it->word());
				++it;
				break;
			case Token::SEMICOLON:
				++it;
				return directive;
			case Token::NEWLINE:
			case Token::LBRACE:
				parseBlock(it, end, directive);
				return directive;
			default:
				throw ParserUnexpectedToken("Unexpected token, expected argument, ';', or block", *it);
		}
	}
	throw ParserUnexpectedEndOfFile("Unexpected end of file, expected argument, ';', or block", *it);
}

void	Parser::parseBlock(TokenStream::const_iterator &it, TokenStream::const_iterator &end, Directive &directive)
{
	while (it->type() != Token::_EOF && it->type() == Token::NEWLINE)
		++it;
	if (it->type() == Token::LBRACE)
	{
		directive.setHasBody(true);
		directive.setBlockErrorInfo(*it);
		++it;
	}
	else if (it->type() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("Unexpected end of file, expected '{' or ';'", *it);
	else
		throw ParserUnexpectedToken("Unexpected token, expected '{' or ';'", *it);
	parseListDirective(it, end, directive.children());
	if (it->type() == Token::RBRACE)
		++it;
	else if (it->type() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("Unexpected end of file, expected '}'", *it);
	else
		throw ParserUnexpectedToken("Unexpected token, expected '}'", *it);
}

void	Parser::parseListDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives)
{
	while (it->type() != Token::_EOF)
	{
		switch (it->type())
		{
			case Token::WORD:
				directives.push_back(parseDirective(it, end));
				break;
			case Token::NEWLINE:
				++it;
				break;
			default:
				return;
		}
	}
}

Parser::ParserUnexpectedToken::ParserUnexpectedToken(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.filename() << ":" << error_info.lineNumber() << ":" << error_info.columnNumber() << ": error: " << description;
	_message = ss.str();
}

Parser::ParserUnexpectedToken::~ParserUnexpectedToken() throw()
{
}

const char	*Parser::ParserUnexpectedToken::what() const throw()
{
	return _message.c_str();
}

Parser::ParserUnexpectedEndOfFile::ParserUnexpectedEndOfFile(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.filename() << ":" << error_info.lineNumber() << ":" << error_info.columnNumber() << ": error: " << description;
	_message = ss.str();
}

Parser::ParserUnexpectedEndOfFile::~ParserUnexpectedEndOfFile() throw()
{
}

const char	*Parser::ParserUnexpectedEndOfFile::what() const throw()
{
	return _message.c_str();
}
