#include "AST.hpp"

AST::AST(): _directives(), _filename()
{
}

AST::~AST()
{
}

AST::AST(const AST &copy): _directives(copy._directives), _filename(copy._filename)
{
}

AST	&AST::operator=(const AST &other)
{
	if (this != &other)
	{
		_directives = other._directives;
		_filename = other._filename;
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

const std::string	&AST::filename() const
{
	return _filename;
}

std::string	&AST::filename()
{
	return _filename;
}
