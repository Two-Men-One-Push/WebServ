#pragma once

#include "IR/AST/AST.hpp"
#include "IR/Directive/Directive.hpp"
#include "IR/TokenStream/TokenStream.hpp"
#include <exception>
#include <string>

class	Parser
{
	private:
		Parser();
		static Directive	parseDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end);
		static void			parseBlock(TokenStream::const_iterator &it, TokenStream::const_iterator &end, Directive &directive);
		static void			parseListDirective(TokenStream::const_iterator &it, TokenStream::const_iterator &end, std::list<Directive> &directives);
	public:
		~Parser();

		static AST	parse(const TokenStream &token_stream);

		class	ParserUnexpectedToken: public std::exception
		{
			private:
				std::string	_message;
			public:
				ParserUnexpectedToken(const std::string &description, const ErrorInfo &error_info);
				virtual ~ParserUnexpectedToken() throw();
				virtual const char	*what() const throw();
		};
		class	ParserUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				ParserUnexpectedEndOfFile(const std::string &description, const ErrorInfo &error_info);
				virtual ~ParserUnexpectedEndOfFile() throw();
				virtual const char	*what() const throw();
		};
};
