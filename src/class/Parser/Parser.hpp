#pragma once

#include "Directive.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include <vector>
#include <exception>
#include <string>
#include <list>
#include "AST.hpp"

class	Parser
{
	private:
		static Directive	parseDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end);
		static void		parseBlock(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives);
		static void		parseListDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives);
	public:
		Parser();
		~Parser();

		static AST	parse(const TokenStream &token_stream);

		class	ParserUnexpectedToken: public std::exception
		{
			private:
				std::string		_message;
			public:
				virtual ~ParserUnexpectedToken() throw();
				ParserUnexpectedToken(const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
		class	ParserUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~ParserUnexpectedEndOfFile() throw();
				ParserUnexpectedEndOfFile(const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
};