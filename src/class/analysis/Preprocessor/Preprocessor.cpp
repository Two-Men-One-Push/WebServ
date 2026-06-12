#include "Preprocessor.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <iostream>
#include <string>
#include <stack>

Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

AST	Preprocessor::preprocess(const AST &ast, DiagnosticContext &diag)
{
	std::stack<std::string>	include_stack;
	AST						preprocessed_ast;

	include_stack.push(ast.getFilename());
	preprocessed_ast.setFilename(ast.getFilename());
	preprocessed_ast.getDirectivesRef() = expand(ast.getDirectives(), include_stack, diag);
	return preprocessed_ast;
}

std::list<Directive>	Preprocessor::expand(const std::list<Directive> &directives, std::stack<std::string> &include_stack, DiagnosticContext &diag)
{
	std::list<Directive>	result;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		if (it->getName().getRawContent() != "include")
		{
			if (it->hasBody())
			{
				Directive expanded(*it);
				expanded.getChildrenRef().clear();
				expanded.getChildrenRef() = expand(it->getChildren(), include_stack, diag);
				result.push_back(expanded);
			}
			else
				result.push_back(*it);
			continue;
		}

		if (it->hasBody())
			diag.report("'include' directive cannot have a body", it->getBlockErrorInfo());

		if (it->getArgs().empty())
		{
			diag.report("'include' directive requires at least one argument", *it);
			continue;
		}

		for (std::vector<Word>::const_iterator arg = it->getArgs().begin(); arg != it->getArgs().end(); ++arg)
		{
			bool circular = false;
			for (std::stack<std::string> tmp = include_stack; !tmp.empty(); tmp.pop())
			{
				if (tmp.top() == arg->getContent())
				{
					diag.report("circular include detected", *arg);
					circular = true;
					break;
				}
			}
			if (circular)
				continue;

			try
			{
				include_stack.push(arg->getContent());
				AST included;
				included.setFilename(arg->getContent());
				included.getDirectivesRef() = expand(
					Parser::parse(Lexer::tokenize(arg->getContent())).getDirectives(),
					include_stack,
					diag
				);
				include_stack.pop();
				result.insert(result.end(), included.getDirectives().begin(), included.getDirectives().end());
			}
			catch (const Lexer::LexerFileOpenFailure &e)
			{
				diag.report(e.what(), *arg);
			}
		}
	}
	return result;
}
