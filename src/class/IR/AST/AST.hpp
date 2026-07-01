#pragma once

#include "IR/Directive/Directive.hpp"
#include <list>
#include <string>

class	AST: public ErrorInfo
{
	private:
		std::list<Directive>	_directives;
	public:
		AST(std::string filename);
		~AST();
		AST(const AST &copy);
		AST	&operator=(const AST &other);

		const std::list<Directive>	&directives() const;
		std::list<Directive>		&directives();
};
