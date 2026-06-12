#pragma once

#include "Parser.hpp"
#include "Lexer.hpp"
#include "AST.hpp"
#include "Directive.hpp"
#include "DiagnosticContext.hpp"
#include <string>
#include <stack>

class Preprocessor
{
	private:
		static std::list<Directive>	expand(const std::list<Directive> &directives, std::stack<std::string> &include_stack, DiagnosticContext &diag);
	public:
		Preprocessor();
		~Preprocessor();

		static AST	preprocess(const AST &ast, DiagnosticContext &diag);
};
