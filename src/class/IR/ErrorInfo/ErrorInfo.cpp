#include "ErrorInfo.hpp"

ErrorInfo::~ErrorInfo()
{
}

ErrorInfo::ErrorInfo()
{
}

ErrorInfo::ErrorInfo(const ErrorInfo &copy): _filename(copy._filename), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

ErrorInfo::ErrorInfo(const std::string &filename, size_t line_number, size_t column_number): _filename(filename), _line_number(line_number), _column_number(column_number)
{
}

void	ErrorInfo::clear()
{
	_filename.clear();
	_line_number = 0;
	_column_number = 0;
}

ErrorInfo	&ErrorInfo::operator=(const ErrorInfo &other)
{
	if (this != &other)
	{
		_filename = other._filename;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return (*this);
}

void	ErrorInfo::setFilename(const std::string &filename)
{
	_filename = filename;
}

void	ErrorInfo::setLineNumber(size_t line_number)
{
	_line_number = line_number;
}

void	ErrorInfo::setColumnNumber(size_t column_number)
{
	_column_number = column_number;
}

const std::string	&ErrorInfo::getFilename() const
{
	return (_filename);
}

size_t	ErrorInfo::getLineNumber() const
{
	return (_line_number);
}

size_t	ErrorInfo::getColumnNumber() const
{
	return (_column_number);
}