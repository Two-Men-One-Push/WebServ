#include "Debug.hpp"
#include "Segment.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Directive.hpp"
#include "AST.hpp"
#include <iostream>
#include <list>

void	Debug::printSegment(std::ostream &os, const Segment &segment)
{
	os << segment.getFilename() << ":" << segment.getLineNumber() << ":" << segment.getColumnNumber() << "		Segment:[" << segment.getRawContent() << " | " << segment.getContent() << "]" << std::endl;
}

void	Debug::printWord(std::ostream &os, const Word &word)
{
	os << word.getFilename() << ":" << word.getLineNumber() << ":" << word.getColumnNumber() << "	Content:[" << word.getRawContent() << " | " << word.getContent() << "]" << std::endl;
	const std::vector<Segment>	&segments = word.getSegments();
	for (size_t i = 0; i < segments.size(); ++i)
	{
		printSegment(os, segments[i]);
	}
}

void	Debug::printToken(std::ostream &os, const Token &token)
{
	os << token.getFilename() << ":" << token.getLineNumber() << ":" << token.getColumnNumber() << "	" << token.getTypeString() << std::endl;
	if (token.getType() == Token::WORD)
		printWord(os, token.getWord());
}

void	Debug::printTokenStream(std::ostream &os, const TokenStream &token_stream)
{
	const std::vector<Token>	&tokens = token_stream.getTokens();

	os << "TokenStream:" << std::endl;
	for (size_t i = 0; i < tokens.size(); ++i)
	{
		const Token &token = tokens[i];
		printToken(os, token);
	}
	os << std::endl;
}

void	Debug::printDirective(std::ostream &os, const Directive &directive, size_t indent)
{
	os << directive.getFilename() << ":" << directive.getLineNumber() << ":" << directive.getColumnNumber() << "	";
	for (size_t i = 0; i < indent; ++i)
		os << "	";
	os << "Directive: [" << directive.getName().getRawContent() << "] [" << directive.getName().getContent() << "]" << std::endl;
	for (size_t i = 0; i < directive.getArgs().size(); ++i)
	{
		const Word &arg = directive.getArgs()[i];
		os << arg.getFilename() << ":" << arg.getLineNumber() << ":" << arg.getColumnNumber() << "	";
		for (size_t j = 0; j < indent + 1; ++j)
			os << "	";
		os << "Arg: [" << arg.getRawContent() << "] [" << arg.getContent() << "]" << std::endl;
	}
	if (directive.getChildren().size() > 0)
	{
		for (size_t i = 0; i < indent + 2; ++i)
			os << "	";
		os << "Children:" << std::endl;
	}
	for (std::list<Directive>::const_iterator it = directive.getChildren().begin(); it != directive.getChildren().end(); ++it)
	{
		printDirective(os, *it, indent + 1);
	}
}

void	Debug::printAST(std::ostream &os, const AST &ast)
{
	const std::list<Directive>	&directives = ast.getDirectives();

	os << "AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		printDirective(os, *it, 0);
	}
	os << std::endl;
}

void	Debug::printPreprocessedAST(std::ostream &os, const AST &ast)
{
	const std::list<Directive>	&directives = ast.getDirectives();

	os << "Preprocessed AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = directives.begin(); it != directives.end(); ++it)
	{
		printDirective(os, *it, 0);
	}
	os << std::endl;
}