#include "Debug/Debug.hpp"
#include "WebServer/WebServer.hpp"
#include "analysis/Lexer/Lexer.hpp"
#include "analysis/Parser/Parser.hpp"
#include "analysis/Preprocessor/Preprocessor.hpp"
#include "analysis/Semantic/Semantic.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include <iostream>
#include "URL/URL.hpp"
#include "Router/Router.hpp"

int main(int argc, char **argv) {
	(void)argc;
	(void)argv;
	WebServer server;
	// if (argc != 3)
	// {
	// 	std::cerr << "Usage: " << argv[0] << " <config_file> <url>" << std::endl;
	// 	return 1;
	// }
	// try
	// {
	// 	DiagnosticContext	diag;
	// 	TokenStream			tokens = Lexer::tokenize(argv[1]);
	// 	AST					ast = Parser::parse(tokens);
	// 	AST					preprocessed = Preprocessor::preprocess(ast, diag);
	// 	Config				config = Semantic::analyseAST(preprocessed, diag);
	// 	//Debug::printConfig(std::cout, config);
	// 	if (diag.hasError())
	// 	{
	// 		std::cerr << "configuration has errors, cannot continue" << std::endl;
	// 		return 1;
	// 	}
	// 	HttpRequest req;
	// 	req.method(GET);
	// 	req.uri(URL(argv[2]));
	// 	Debug::printURL(std::cout, req.uri());
	// 	Ressource ressource = Router::resolveRessource(req, config.http().servers()[0]);
	// 	Debug::printRessource(std::cout, ressource);
	// }
	// catch (const std::exception &e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// 	return 1;
	// }
	// return 0;
}
