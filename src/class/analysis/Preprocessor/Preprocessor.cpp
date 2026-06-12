#include "Preprocessor.hpp"
#include "Parser.hpp"
#include "Lexer.hpp"
#include "Debug.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <stack>
#include <iostream>

Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

AST	Preprocessor::preprocess(const AST &ast)
{
	std::stack<std::string>	include_stack;
	AST	preprocessed_ast;

	include_stack.push(ast.getFilename());
	preprocessed_ast.setFilename(ast.getFilename());
	preprocessed_ast.getDirectivesRef() = expand(ast.getDirectives(), include_stack);
	return (preprocessed_ast);
}

std::list<Directive>	Preprocessor::expand(const std::list<Directive> &directives, std::stack<std::string> &include_stack)
{
	std::list<Directive>	preprocessed_directives;

	for (std::list<Directive>::const_iterator directive_it = directives.begin(); directive_it != directives.end(); ++directive_it)
	{
		if (directive_it->getName().getRawContent().compare("include") == 0)
		{
			bool	canProcess = true;

			if (directive_it->hasBody())
			{
				std::cerr << PreprocessorIllegalBody("Include directive cannot have a body", directive_it->getBlockErrorInfo()).what() << std::endl;
			}
			if (directive_it->getArgs().size() < 1)
			{
				std::cerr << PreprocessorInvalidArguments("Include directive requires at least one argument", *directive_it).what() << std::endl;
				canProcess = false;
			}
			if (canProcess)
			{
				for (std::vector<Word>::const_iterator arg_it = directive_it->getArgs().begin(); arg_it != directive_it->getArgs().end(); ++arg_it)
				{
					bool circular_include = false;
					for (std::stack<std::string> tmp_stack = include_stack; !tmp_stack.empty(); tmp_stack.pop())
					{
						if (tmp_stack.top() == arg_it->getContent())
						{
							std::cerr << PreprocessorInvalidArguments("Circular include detected", *arg_it).what() << std::endl;
							circular_include = true;
							break;
						}
					}
					if (circular_include)
						continue;
					try
					{
						include_stack.push(arg_it->getContent());
						AST	included_ast;
						included_ast.setFilename(arg_it->getContent());
						included_ast.getDirectivesRef() = expand(Parser::parse(Lexer::tokenize(arg_it->getContent())).getDirectives(), include_stack);
						include_stack.pop();
						preprocessed_directives.insert(preprocessed_directives.end(), included_ast.getDirectives().begin(), included_ast.getDirectives().end());
					}
					catch (const Lexer::LexerFileOpenFailure &e)
					{
						std::cerr << PreprocessorInvalidArguments(e.what(), *arg_it).what() << std::endl;
					}
				}
			}
		}
		else if (directive_it->hasBody())
		{
			Directive	preprocessed_directive(*directive_it);

			preprocessed_directive.getChildrenRef().clear();
			preprocessed_directive.getChildrenRef() = expand(directive_it->getChildren(), include_stack);
			preprocessed_directives.push_back(preprocessed_directive);
		}
		else
		{
			preprocessed_directives.push_back(*directive_it);
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