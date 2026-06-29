#pragma once

#include "IR/Directive/Directive.hpp"
#include <list>
#include <string>

class	AST
{
	private:
		std::list<Directive>	_directives;
		std::string				_filename;
	public:
		AST();
		~AST();
		AST(const AST &copy);
		AST	&operator=(const AST &other);

		const std::list<Directive>	&directives() const;
		std::list<Directive>		&directives();
		const std::string			&filename() const;
		std::string					&filename();
};
