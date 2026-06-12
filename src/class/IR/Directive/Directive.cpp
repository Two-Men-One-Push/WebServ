#include "Directive.hpp"

Directive::~Directive()
{
}

Directive::Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number): ErrorInfo(filename, line_number, column_number), _name(name), _args(), _has_body(false), _children()
{
}

Directive::Directive(const Directive &copy): ErrorInfo(copy), _name(copy._name), _args(copy._args), _has_body(copy._has_body), _children(copy._children)
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
		ErrorInfo::operator=(other);
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

const ErrorInfo	&Directive::getBlockErrorInfo() const
{
	return (_block_error_info);
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

void	Directive::setBlockErrorInfo(const ErrorInfo &error_info)
{
	_block_error_info = error_info;
}