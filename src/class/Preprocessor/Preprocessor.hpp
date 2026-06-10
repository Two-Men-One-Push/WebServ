#pragma once

#include "Parser.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Directive.hpp"
#include <exception>
#include <string>
#include <stack>

class Preprocessor
{
	private:
		static AST	expand(const AST &ast, std::stack<std::string> &include_stack);
		static std::list<Directive>	expand(const Directive &directive, std::stack<std::string> &include_stack);
	public:
		Preprocessor();
		~Preprocessor();

		static AST	preprocess(const AST &ast);
	
		class	PreprocessorIncludeDirectiveInvalidArguments: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~PreprocessorIncludeDirectiveInvalidArguments() throw();
				PreprocessorIncludeDirectiveInvalidArguments(const std::string &description, const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
		class	PreprocessorIncludeDirectiveIllegalBody: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~PreprocessorIncludeDirectiveIllegalBody() throw();
				PreprocessorIncludeDirectiveIllegalBody(const std::string &description, const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
};