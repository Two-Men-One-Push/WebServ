#include <iostream>
#include <sstream>
#include "Lexer.hpp"
#include "Parser.hpp"
#include "Debug.hpp"
#include "Preprocessor.hpp"
#include "Semantic.hpp"

int	main(int argc, char **argv) {
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return (1);
	}
	try {
		TokenStream	token_stream = Lexer::tokenize(argv[1]);
		AST	parser = Parser::parse(token_stream);
		Debug::printAST(std::cout, parser);
		AST	preprocessed_ast = Preprocessor::preprocess(parser);
		Debug::printPreprocessedAST(std::cout, preprocessed_ast);
	}
	catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
	return (0);
}