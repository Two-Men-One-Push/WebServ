#include "Logger/Logger.hpp"
#include "WebServer/WebServer.hpp"
#include "analysis/Lexer/Lexer.hpp"
#include "analysis/Parser/Parser.hpp"
#include "analysis/Preprocessor/Preprocessor.hpp"
#include "analysis/Semantic/Semantic.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>

int parseVerbose(const std::string &value) {
	if (value == "silent")
		Logger::level(Logger::LOG_SILENT);
	else if (value == "error")
		Logger::level(Logger::LOG_ERROR);
	else if (value == "warn")
		Logger::level(Logger::LOG_WARN);
	else if (value == "info")
		Logger::level(Logger::LOG_INFO);
	else if (value == "debug")
		Logger::level(Logger::LOG_DEBUG);
	else
		return 1;
	return 0;
}

int parseShortVerbose(const std::string &value) {
	size_t vCount = 0;
	for (std::string::const_iterator it = value.begin() + 1; it != value.end(); ++it) {
		if (*it == 'v') ++vCount;
		else return 1;
	}
	if (vCount == 1) Logger::level(Logger::LOG_INFO);
	else Logger::level(Logger::LOG_DEBUG);
	return 0;
}

void help(const std::string &binName) {
	std::cout << "Usage: " << binName << " [OPTIONS] <config_file>\n\
Options:\n\
  -t, --test\n\
      Test the configuration and exit\n\
\n\
  -s, --silent\n\
      Suppress non-error output\n\
\n\
  -v, --verbose\n\
      Increase verbosity\n\
\n\
  --verbose <LEVEL>\n\
      Set verbosity level: silent, error, warn, info, debug" << std::endl;

	std::exit(0);
}

int parseOption(int ac, char **av, std::string &configFile, bool &testOnly) {
	for (int i = 1; i < ac; ++i) {
		std::string arg(av[i]);
		if (arg.compare("--verbose") == 0) {
			if (i == ac - 1) return 1;
			++i;
			if (parseVerbose(av[i])) return 1;
		} else if (arg.compare(0, 2, "-v") == 0) {
			if (parseShortVerbose(arg)) return 1;
		} else if (arg.compare("--silent") == 0 || arg.compare("-s") == 0) {
			Logger::level(Logger::LOG_SILENT);
		} else if (arg.compare("--test") == 0 || arg.compare("-t") == 0) {
			testOnly = true;
		} else if (arg.compare("-h") == 0) {
			help(av[0]);
		} else {
			if (configFile.empty()) configFile = arg;
			else return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	signal(SIGPIPE, SIG_IGN);
	std::string configFile;
	bool testOnly = false;
	if (parseOption(argc, argv, configFile, testOnly) || configFile.empty()) {
		std::cerr << "Usage: " << argv[0] << " [OPTIONS] <config_file>" << std::endl;
		return 1;
	}
	try {
		DiagnosticContext diag;
		TokenStream tokens = Lexer::tokenize(configFile);
		AST ast = Parser::parse(tokens);
		AST preprocessed = Preprocessor::preprocess(ast, diag);
		Config config = Semantic::analyseAST(preprocessed, diag);
		if (diag.hasError()) {
			Logger::error() << "configuration has errors, cannot continue" << std::endl;
			return 1;
		}

		if (!testOnly) WebServer server(config);
	} catch (const std::exception &e) {
		Logger::error() << e.what() << std::endl;
		return 1;
	}
	return 0;
}
