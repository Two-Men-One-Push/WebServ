#include "Lexer.hpp"
#include "Parser.hpp"
#include "Preprocessor.hpp"
#include "Semantic.hpp"
#include "DiagnosticContext.hpp"
#include <iostream>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	try
	{
		DiagnosticContext	diag;
		TokenStream			tokens = Lexer::tokenize(argv[1]);
		AST					ast = Parser::parse(tokens);
		AST					preprocessed = Preprocessor::preprocess(ast, diag);
		Config				config = Semantic::analyseAST(preprocessed, diag);
		if (diag.hasError())
		{
			std::cerr << "configuration has errors, cannot continue" << std::endl;
			return 1;
		}
		(void)config;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
