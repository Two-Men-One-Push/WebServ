#include "Logger/Logger.hpp"
#include "WebServer/WebServer.hpp"
#include "analysis/Lexer/Lexer.hpp"
#include "analysis/Parser/Parser.hpp"
#include "analysis/Preprocessor/Preprocessor.hpp"
#include "analysis/Semantic/Semantic.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
#include <csignal>
#include <iostream>

int main(int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	if (argc != 2) {
		Logger::error() << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	try {
		DiagnosticContext diag;
		TokenStream tokens = Lexer::tokenize(argv[1]);
		AST ast = Parser::parse(tokens);
		AST preprocessed = Preprocessor::preprocess(ast, diag);
		Config config = Semantic::analyseAST(preprocessed, diag);
		if (diag.hasError()) {
			Logger::error() << "configuration has errors, cannot continue" << std::endl;
			return 1;
		}

		WebServer server(config);
	} catch (const std::exception &e) {
		Logger::error() << e.what() << std::endl;
		return 1;
	}
	return 0;
}
