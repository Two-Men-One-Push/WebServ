#include "Directive.hpp"
#include "Segment.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iterator>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <list>

Parser::Parser()
{
}

Parser::~Parser()
{
}

/*
ast := list_directive EOF
*/
AST	Parser::parse(const TokenStream &token_stream)
{
	AST	ast;
	TokenStream::const_iterator	it = token_stream.begin();
	TokenStream::const_iterator	end = token_stream.end();

	ast.setFilename(token_stream.getFilename());
	parseListDirective(it, end, ast.getDirectivesRef());
	if (it->getType() != Token::_EOF)
		throw ParserUnexpectedToken("Unexpected token expected directive or EOF", *it);
	return (ast);
}

/*
directive := WORD WORD* (SEMICOLON | block)
*/
Directive	Parser::parseDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end)
{
	Directive	directive(it->getWord(), it->getFilename(), it->getLineNumber(), it->getColumnNumber());
	it++;

	while (it->getType() != Token::_EOF)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directive.addArg(it->getWord());
				it++;
				break;
			case Token::SEMICOLON:
				it++;
				return (directive);
			case Token::NEWLINE:
			case Token::LBRACE:
				directive.setHasBody(true);
				parseBlock(it, end, directive.getChildrenRef());
				return (directive);
			default:
				throw ParserUnexpectedToken("Unexpected token expected argument or directive end or block", *it);
		}
	}
	throw ParserUnexpectedEndOfFile("Unexpected end of file expected argument or directive end or block", *it);
}

/*
block := NEWLINE* LBRACE list_directive RBRACE
*/
void	Parser::parseBlock(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives)
{
	while (it->getType() != Token::_EOF)
	{
		if (it->getType() == Token::NEWLINE)
			it++;
		else
			break;
	}
	if (it->getType() == Token::LBRACE)
		it++;
	else if (it->getType() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("Unexpected end of file expected '{' or ';'", *it);
	else
		throw ParserUnexpectedToken("Unexpected token expected '{' or ';'", *it);
	parseListDirective(it, end, directives);
	if (it->getType() == Token::RBRACE)
		it++;
	else if (it->getType() == Token::_EOF)
		throw ParserUnexpectedEndOfFile("Unexpected end of file expected '}'", *it);
	else
		throw ParserUnexpectedToken("Unexpected token expected '}'", *it);
}

/*
list_directive := directive*
*/
void	Parser::parseListDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives)
{
	while (it->getType() != Token::_EOF)
	{
		switch (it->getType())
		{
			case Token::WORD:
				directives.push_back(parseDirective(it, end));
				break;
			case Token::NEWLINE:
				it++;
				break;
			default:
				return;
		}
	}
}

Parser::ParserUnexpectedToken::~ParserUnexpectedToken() throw()
{
}

Parser::ParserUnexpectedToken::ParserUnexpectedToken(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream	ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

Parser::ParserUnexpectedEndOfFile::~ParserUnexpectedEndOfFile() throw()
{
}

Parser::ParserUnexpectedEndOfFile::ParserUnexpectedEndOfFile(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream	ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Parser::ParserUnexpectedToken::what() const throw()
{
	return (_message.c_str());
}

const char	*Parser::ParserUnexpectedEndOfFile::what() const throw()
{
	return (_message.c_str());
}