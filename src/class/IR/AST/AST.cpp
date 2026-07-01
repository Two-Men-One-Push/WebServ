#include "AST.hpp"
#include <string>

AST::AST(std::string filename): ErrorInfo(filename, 1, 1), _directives()
{
}

AST::~AST()
{
}

AST::AST(const AST &copy): ErrorInfo(copy), _directives(copy._directives)
{
}

AST	&AST::operator=(const AST &other)
{
	if (this != &other)
	{
		ErrorInfo::operator=(other);
		_directives = other._directives;
	}
	return *this;
}

const std::list<Directive>	&AST::directives() const
{
	return _directives;
}

std::list<Directive>	&AST::directives()
{
	return _directives;
}