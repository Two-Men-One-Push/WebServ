#pragma once

#include "Segment.hpp"
#include "Word.hpp"
#include "Token.hpp"
#include "TokenStream.hpp"
#include "Directive.hpp"
#include "AST.hpp"
#include <ostream>

class	Debug
{
	public:
		static void	printSegment(std::ostream &os, const Segment &segment);
		static void	printWord(std::ostream &os, const Word &word);
		static void	printToken(std::ostream &os, const Token &token);
		static void	printTokenStream(std::ostream &os, const TokenStream &token_stream);
		static void	printDirective(std::ostream &os, const Directive &directive, size_t indent);
		static void	printAST(std::ostream &os, const AST &ast);
		static void	printPreprocessedAST(std::ostream &os, const AST &ast);
};