#include "Directive.hpp"

Directive::Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number): ErrorInfo(filename, line_number, column_number), _name(name), _args(), _has_body(false), _block_error_info(), _children()
{
}

Directive::~Directive()
{
}

Directive::Directive(const Directive &copy): ErrorInfo(copy), _name(copy._name), _args(copy._args), _has_body(copy._has_body), _block_error_info(copy._block_error_info), _children(copy._children)
{
}

Directive	&Directive::operator=(const Directive &other)
{
	if (this != &other)
	{
		ErrorInfo::operator=(other);
		_name = other._name;
		_args = other._args;
		_has_body = other._has_body;
		_block_error_info = other._block_error_info;
		_children = other._children;
	}
	return *this;
}

const Word	&Directive::name() const
{
	return _name;
}

const std::vector<Word>	&Directive::args() const
{
	return _args;
}

bool	Directive::hasBody() const
{
	return _has_body;
}

const ErrorInfo	&Directive::blockErrorInfo() const
{
	return _block_error_info;
}

const std::list<Directive>	&Directive::children() const
{
	return _children;
}

std::list<Directive>	&Directive::children()
{
	return _children;
}

void	Directive::addArg(const Word &arg)
{
	_args.push_back(arg);
}

void	Directive::addChild(const Directive &child)
{
	_children.push_back(child);
}

void	Directive::setHasBody(bool has_body)
{
	_has_body = has_body;
}

void	Directive::setBlockErrorInfo(const ErrorInfo &error_info)
{
	_block_error_info = error_info;
}
