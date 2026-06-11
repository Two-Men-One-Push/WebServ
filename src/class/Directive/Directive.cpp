#include "Directive.hpp"

Directive::~Directive()
{
}

Directive::Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number): _name(name), _args(), _has_body(false), _children(), _filename(filename), _line_number(line_number), _column_number(column_number)
{
}

Directive::Directive(const Directive &copy): _name(copy._name), _args(copy._args), _has_body(copy._has_body), _children(copy._children), _filename(copy._filename), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

Directive	&Directive::operator=(const Directive &other)
{
	if (this != &other)
	{
		_name = other._name;
		_args = other._args;
		_has_body = other._has_body;
		_children = other._children;
		_filename = other._filename;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return (*this);
}

const Word	&Directive::getName() const
{
	return (_name);
}

const std::vector<Word>	&Directive::getArgs() const
{
	return (_args);
}

const std::list<Directive>	&Directive::getChildren() const
{
	return (_children);
}

std::list<Directive>	&Directive::getChildrenRef()
{
	return (_children);
}

const std::string	&Directive::getFilename() const
{
	return (_filename);
}

size_t	Directive::getLineNumber() const
{
	return (_line_number);
}

size_t	Directive::getColumnNumber() const
{
	return (_column_number);
}

bool	Directive::hasBody() const
{
	return (_has_body);
}

void	Directive::setHasBody(bool has_body)
{
	_has_body = has_body;
}

void	Directive::addArg(const Word &arg)
{
	_args.push_back(arg);
}

void	Directive::addChild(const Directive &child)
{
	_children.push_back(child);
}