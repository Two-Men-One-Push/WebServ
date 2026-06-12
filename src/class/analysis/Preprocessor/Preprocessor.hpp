#pragma once

#include "Parser.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Directive.hpp"
#include "ErrorInfo.hpp"
#include <exception>
#include <string>
#include <stack>

class Preprocessor
{
	private:
		static std::list<Directive>	expand(const std::list<Directive> &directives, std::stack<std::string> &include_stack, bool &error_occurred);
	public:
		Preprocessor();
		~Preprocessor();

		static AST	preprocess(const AST &ast);
	
		class	PreprocessorError: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~PreprocessorError() throw();
				PreprocessorError(const std::string &description);
				virtual const char	*what() const throw();
		};
		class	PreprocessorInvalidArguments: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~PreprocessorInvalidArguments() throw();
				PreprocessorInvalidArguments(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
		class	PreprocessorIllegalBody: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~PreprocessorIllegalBody() throw();
				PreprocessorIllegalBody(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
};