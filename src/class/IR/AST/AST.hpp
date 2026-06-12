#pragma once

#include "Directive.hpp"
#include <list>

class AST
{
	std::list<Directive>	_directives;
	std::string				_filename;

	public:
		AST();
		~AST();
		AST(const AST &copy);
		AST	&operator=(const AST &other);

		const std::list<Directive>	&getDirectives() const;
		std::list<Directive>		&getDirectivesRef();
		const std::string			&getFilename() const;

		void	setFilename(const std::string &filename);
};