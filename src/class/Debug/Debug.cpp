#include "Debug.hpp"
#include <list>

void	Debug::printSegment(std::ostream &os, const Segment &segment)
{
	os << segment.filename() << ":" << segment.lineNumber() << ":" << segment.columnNumber()
	   << "\t\tSegment:[" << segment.rawContent() << " | " << segment.content() << "]" << std::endl;
}

void	Debug::printWord(std::ostream &os, const Word &word)
{
	os << word.filename() << ":" << word.lineNumber() << ":" << word.columnNumber()
	   << "\tContent:[" << word.rawContent() << " | " << word.content() << "]" << std::endl;
	const std::vector<Segment>	&segs = word.segments();
	for (size_t i = 0; i < segs.size(); ++i)
		printSegment(os, segs[i]);
}

void	Debug::printToken(std::ostream &os, const Token &token)
{
	os << token.filename() << ":" << token.lineNumber() << ":" << token.columnNumber()
	   << "\t" << token.typeString() << std::endl;
	if (token.type() == Token::WORD)
		printWord(os, token.word());
}

void	Debug::printTokenStream(std::ostream &os, const TokenStream &token_stream)
{
	const std::vector<Token>	&toks = token_stream.tokens();

	os << "TokenStream:" << std::endl;
	for (size_t i = 0; i < toks.size(); ++i)
		printToken(os, toks[i]);
	os << std::endl;
}

void	Debug::printDirective(std::ostream &os, const Directive &directive, size_t indent)
{
	os << directive.filename() << ":" << directive.lineNumber() << ":" << directive.columnNumber() << "\t";
	for (size_t i = 0; i < indent; ++i)
		os << "\t";
	os << "Directive: [" << directive.name().rawContent() << "] [" << directive.name().content() << "]" << std::endl;
	for (size_t i = 0; i < directive.args().size(); ++i)
	{
		const Word	&arg = directive.args()[i];
		os << arg.filename() << ":" << arg.lineNumber() << ":" << arg.columnNumber() << "\t";
		for (size_t j = 0; j < indent + 1; ++j)
			os << "\t";
		os << "Arg: [" << arg.rawContent() << "] [" << arg.content() << "]" << std::endl;
	}
	if (!directive.children().empty())
	{
		for (size_t i = 0; i < indent + 2; ++i)
			os << "\t";
		os << "Children:" << std::endl;
	}
	for (std::list<Directive>::const_iterator it = directive.children().begin(); it != directive.children().end(); ++it)
		printDirective(os, *it, indent + 1);
}

void	Debug::printAST(std::ostream &os, const AST &ast)
{
	os << "AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
		printDirective(os, *it, 0);
	os << std::endl;
}

void	Debug::printPreprocessedAST(std::ostream &os, const AST &ast)
{
	os << "Preprocessed AST:" << std::endl;
	for (std::list<Directive>::const_iterator it = ast.directives().begin(); it != ast.directives().end(); ++it)
		printDirective(os, *it, 0);
	os << std::endl;
}
