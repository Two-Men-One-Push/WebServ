#include "AST.hpp"

AST::AST(): _directives()
{
}

AST::~AST()
{
}

AST::AST(const AST &copy): _directives(copy._directives)
{
}

AST	&AST::operator=(const AST &other)
{
	if (this != &other)
	{
		_directives = other._directives;
	}
	return *this;
}

const std::list<Directive>	&AST::getDirectives() const
{
	return _directives;
}

std::list<Directive>	&AST::getDirectivesRef()
{
	return _directives;
}

const std::string	&AST::getFilename() const
{
	return _filename;
}

void	AST::setFilename(const std::string &filename)
{
	_filename = filename;
}