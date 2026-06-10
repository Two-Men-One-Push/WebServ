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

			if (it->getChildren().size() > 0)
			{
				throw PreprocessorIncludeDirectiveIllegalBody("Include directive cannot have a body", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
			}
			if (args.size() < 1)
			{
				throw PreprocessorIncludeDirectiveInvalidArguments("Include directive requires at least one argument", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
			}
			for (size_t i = 0; i < args.size(); ++i)
			{
				for (std::stack<std::string> tmp_stack = include_stack; !tmp_stack.empty(); tmp_stack.pop())
				{
					if (tmp_stack.top() == args[i].getContent())
					{
						throw PreprocessorIncludeDirectiveInvalidArguments("Circular include detected", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
					}
				}
				include_stack.push(args[i].getContent());
				AST	included_ast = expand(Parser::parse(Lexer::tokenize(args[i].getContent())), include_stack);
				include_stack.pop();
				preprocessed_ast.getDirectivesRef().insert(preprocessed_ast.getDirectivesRef().end(), included_ast.getDirectives().begin(), included_ast.getDirectives().end());
			}
		}
		else if (it->getChildren().size() > 0)
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

			if (it->getChildren().size() > 0)
			{
				throw PreprocessorIncludeDirectiveIllegalBody("Include directive cannot have a body", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
			}
			if (args.size() < 1)
			{
				throw PreprocessorIncludeDirectiveInvalidArguments("Include directive requires at least one argument", it->getFilename(), it->getLineNumber(), it->getColumnNumber());
			}
			for (size_t i = 0; i < args.size(); ++i)
			{
				include_stack.push(args[i].getContent());
				AST	included_ast = expand(Parser::parse(Lexer::tokenize(args[i].getContent())), include_stack);
				include_stack.pop();
				preprocessed_directives.insert(preprocessed_directives.end(), included_ast.getDirectives().begin(), included_ast.getDirectives().end());
			}
		}
		else if (it->getChildren().size() > 0)
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

Preprocessor::PreprocessorIncludeDirectiveInvalidArguments::~PreprocessorIncludeDirectiveInvalidArguments() throw()
{
}

Preprocessor::PreprocessorIncludeDirectiveInvalidArguments::PreprocessorIncludeDirectiveInvalidArguments(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << description;
	_message = ss.str();
}

const char	*Preprocessor::PreprocessorIncludeDirectiveInvalidArguments::what() const throw()
{
	return _message.c_str();
}

Preprocessor::PreprocessorIncludeDirectiveIllegalBody::~PreprocessorIncludeDirectiveIllegalBody() throw()
{
}

Preprocessor::PreprocessorIncludeDirectiveIllegalBody::PreprocessorIncludeDirectiveIllegalBody(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << " " << description;
	_message = ss.str();
}

const char	*Preprocessor::PreprocessorIncludeDirectiveIllegalBody::what() const throw()
{
	return _message.c_str();
}