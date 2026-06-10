#pragma once

#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include "Parser.hpp"
#include <vector>

class	Semantic
{
	private:
		
		Server		analyseServer(const std::list<Directive> &directives);
		Location	analyseLocation(const std::list<Directive> &directives);
	public:
		Semantic();
		~Semantic();

		Config	analyseAST(const AST &ast);
};