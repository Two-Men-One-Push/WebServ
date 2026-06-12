#include "Preprocessor.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"

Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

AST	Preprocessor::preprocess(const AST &ast, DiagnosticContext &diag)
{
	std::stack<std::string>	include_stack;
	AST						result;

	include_stack.push(ast.filename());
	result.filename() = ast.filename();
	result.directives() = expand(ast.directives(), include_stack, diag);
	return result;
}

std::list<Directive>	Preprocessor::expand(const std::list<Directive> &directives, std::stack<std::string> &include_stack, DiagnosticContext &diag)
{
	std::list<Directive>	result;

	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		if (it->name().rawContent() != "include")
		{
			if (it->hasBody())
			{
				Directive expanded(*it);
				expanded.children().clear();
				expanded.children() = expand(it->children(), include_stack, diag);
				result.push_back(expanded);
			}
			else
				result.push_back(*it);
			continue;
		}

		if (it->hasBody())
			diag.report("'include' directive cannot have a body", it->blockErrorInfo());

		if (it->args().empty())
		{
			diag.report("'include' directive requires at least one argument", *it);
			continue;
		}

		for (std::vector<Word>::const_iterator arg = it->args().begin(); arg != it->args().end(); ++arg)
		{
			bool circular = false;
			for (std::stack<std::string> tmp = include_stack; !tmp.empty(); tmp.pop())
			{
				if (tmp.top() == arg->content())
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
				include_stack.push(arg->content());
				AST included;
				included.filename() = arg->content();
				included.directives() = expand(
					Parser::parse(Lexer::tokenize(arg->content())).directives(),
					include_stack,
					diag
				);
				include_stack.pop();
				result.insert(result.end(), included.directives().begin(), included.directives().end());
			}
			catch (const Lexer::LexerFileOpenFailure &e)
			{
				diag.report(e.what(), *arg);
			}
		}
	}
	return result;
}
