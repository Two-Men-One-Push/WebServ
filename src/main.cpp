#include "analysis/Lexer/Lexer.hpp"
#include "analysis/Parser/Parser.hpp"
#include "analysis/Preprocessor/Preprocessor.hpp"
#include "analysis/Semantic/Semantic.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
#include "Debug/Debug.hpp"
#include <iostream>
#include "URL/URL.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
		return 1;
	}
	URL	url(argv[1]);
	std::cout << "format: " << url.formatStr() << std::endl;
	std::cout << "scheme: " << url.scheme() << std::endl;
	std::cout << "user: " << url.user() << std::endl;
	std::cout << "host: " << url.host() << std::endl;
	std::cout << "port: " << url.port() << std::endl;
	for (size_t i = 0; i < url.path().size(); ++i)
	{
		std::cout << "path[" << i << "]: " << url.path()[i] << std::endl;
	}
	std::cout << "raw query: " << url.rawQuery() << std::endl;
	std::cout << "query: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = url.query().begin(); it != url.query().end(); ++it)
	{
		std::cout << "  " << it->first << " = " << it->second << std::endl;
	}
	std::cout << "raw fragment: " << url.rawFragment() << std::endl;
	std::cout << "fragment: " << url.fragment() << std::endl;
	std::string decoded;
	if (URL::decode(decoded, argv[1]))
	{
		std::cout << "decoded: " << decoded << std::endl;
	}
	else
	{
		std::cout << "failed to decode" << std::endl;
	}
	return 0;
	try
	{
		DiagnosticContext	diag;
		TokenStream			tokens = Lexer::tokenize(argv[1]);
		AST					ast = Parser::parse(tokens);
		AST					preprocessed = Preprocessor::preprocess(ast, diag);
		Config				config = Semantic::analyseAST(preprocessed, diag);
		//Debug::printConfig(std::cout, config);
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