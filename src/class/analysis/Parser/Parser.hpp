#pragma once

#include "Directive.hpp"
#include "Token.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include <vector>
#include <exception>
#include <string>
#include <list>

class	Parser
{
	private:
		static Directive	parseDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end);
		static void			parseBlock(TokenStream::const_iterator &it, TokenStream::const_iterator &end, Directive &directive);
		static void			parseListDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives);
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
				ParserUnexpectedToken(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
		class	ParserUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~ParserUnexpectedEndOfFile() throw();
				ParserUnexpectedEndOfFile(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
};