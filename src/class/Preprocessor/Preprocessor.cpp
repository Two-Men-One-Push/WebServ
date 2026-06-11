#include "Preprocessor.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include <sstream>
#include "Debug.hpp"
#include <iostream>
#include <string>
#include <stack>

Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

AST	Preprocessor::preprocess(const AST &ast)
{
	std::stack<std::string>	include_stack;

	include_stack.push(ast.getFilename());
	return (expand(ast, include_stack));
}

AST	Preprocessor::expand(const AST &ast, std::stack<std::string> &include_stack)
{
	AST						preprocessed_ast;

	for (std::list<Directive>::const_iterator it = ast.getDirectives().begin(); it != ast.getDirectives().end(); ++it)
	{
		if (it->getName().getRawContent().compare("include") == 0)
		{
			const std::vector<Word>	&args = it->getArgs();

			if (args.size() < 1)
			{
				throw PreprocessorInvalidArguments("Include directive requires at least one argument", *it);
			}
			else if (it->hasBody())
			{
				throw PreprocessorIllegalBody("Include directive cannot have a body", *it);
			}
			for (size_t i = 0; i < args.size(); ++i)
			{
				for (std::stack<std::string> tmp_stack = include_stack; !tmp_stack.empty(); tmp_stack.pop())
				{
					if (tmp_stack.top() == args[i].getContent())
					{
						throw PreprocessorInvalidArguments("Circular include detected", *it);
					}
				}
				try
				{
					include_stack.push(args[i].getContent());
					AST	included_ast = expand(Parser::parse(Lexer::tokenize(args[i].getContent())), include_stack);
					include_stack.pop();
					preprocessed_ast.getDirectivesRef().insert(preprocessed_ast.getDirectivesRef().end(), included_ast.getDirectives().begin(), included_ast.getDirectives().end());
				}
				catch (const Lexer::LexerFileOpenFailure &e)
				{
					throw PreprocessorInvalidArguments(e.what(), *it);
				}
			}
		}
		else if (it->hasBody())
		{
			Directive	preprocessed_directive = *it;

			preprocessed_directive.getChildrenRef().clear();
			preprocessed_directive.getChildrenRef() = expand(*it, include_stack);
			preprocessed_ast.getDirectivesRef().push_back(preprocessed_directive);
		}
		else
		{
			preprocessed_ast.getDirectivesRef().push_back(*it);
		}
	}
	return (preprocessed_ast);
}

std::list<Directive>	Preprocessor::expand(const Directive &directive, std::stack<std::string> &include_stack)
{
	std::list<Directive>	preprocessed_directives;

	for (std::list<Directive>::const_iterator it = directive.getChildren().begin(); it != directive.getChildren().end(); ++it)
	{
		if (it->getName().getRawContent().compare("include") == 0)
		{
			const std::vector<Word>	&args = it->getArgs();

			if (args.size() < 1)
			{
				throw PreprocessorInvalidArguments("Include directive requires at least one argument", *it);
			}
			else if (it->hasBody())
			{
				throw PreprocessorIllegalBody("Include directive cannot have a body", *it);
			}
			for (size_t i = 0; i < args.size(); ++i)
			{
				try
				{
					include_stack.push(args[i].getContent());
					AST	included_ast = expand(Parser::parse(Lexer::tokenize(args[i].getContent())), include_stack);
					include_stack.pop();
					preprocessed_directives.insert(preprocessed_directives.end(), included_ast.getDirectives().begin(), included_ast.getDirectives().end());
				}
				catch (const Lexer::LexerFileOpenFailure &e)
				{
					throw PreprocessorInvalidArguments(e.what(), *it);
				}
			}
		}
		else if (it->hasBody())
		{
			Directive	preprocessed_directive = *it;

			preprocessed_directive.getChildrenRef().clear();
			preprocessed_directive.getChildrenRef() = expand(*it, include_stack);
			preprocessed_directives.push_back(preprocessed_directive);
		}
		else
		{
			preprocessed_directives.push_back(*it);
		}
	}
	return (preprocessed_directives);
}

Preprocessor::PreprocessorInvalidArguments::~PreprocessorInvalidArguments() throw()
{
}

Preprocessor::PreprocessorInvalidArguments::PreprocessorInvalidArguments(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Preprocessor::PreprocessorInvalidArguments::what() const throw()
{
	return _message.c_str();
}

Preprocessor::PreprocessorIllegalBody::~PreprocessorIllegalBody() throw()
{
}

Preprocessor::PreprocessorIllegalBody::PreprocessorIllegalBody(const std::string &description, const ErrorInfo &error_info): _message()
{
	std::stringstream ss;
	ss << error_info.getFilename() << ":" << error_info.getLineNumber() << ":" << error_info.getColumnNumber() << " " << description;
	_message = ss.str();
}

const char	*Preprocessor::PreprocessorIllegalBody::what() const throw()
{
	return _message.c_str();
}