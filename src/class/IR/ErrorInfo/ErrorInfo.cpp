#include "ErrorInfo.hpp"

ErrorInfo::ErrorInfo(): _filename(), _line_number(0), _column_number(0)
{
}

ErrorInfo::~ErrorInfo()
{
}

ErrorInfo::ErrorInfo(const ErrorInfo &copy): _filename(copy._filename), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

ErrorInfo::ErrorInfo(const std::string &filename, size_t line_number, size_t column_number): _filename(filename), _line_number(line_number), _column_number(column_number)
{
}

ErrorInfo	&ErrorInfo::operator=(const ErrorInfo &other)
{
	if (this != &other)
	{
		_filename = other._filename;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return *this;
}

void	ErrorInfo::clear()
{
	_filename.clear();
	_line_number = 0;
	_column_number = 0;
}

const std::string	&ErrorInfo::filename() const
{
	return _filename;
}

std::string	&ErrorInfo::filename()
{
	return _filename;
}

size_t	ErrorInfo::lineNumber() const
{
	return _line_number;
}

size_t	&ErrorInfo::lineNumber()
{
	return _line_number;
}

size_t	ErrorInfo::columnNumber() const
{
	return _column_number;
}

size_t	&ErrorInfo::columnNumber()
{
	return _column_number;
}
