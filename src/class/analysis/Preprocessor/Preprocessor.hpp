#pragma once

#include "IR/AST/AST.hpp"
#include "IR/Directive/Directive.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
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
