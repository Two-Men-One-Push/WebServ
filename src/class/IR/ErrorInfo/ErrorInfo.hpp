#pragma once

#include <string>

class	ErrorInfo
{
	private:
		std::string	_filename;
		size_t		_line_number;
		size_t		_column_number;
	public:
		virtual ~ErrorInfo();
		ErrorInfo();
		ErrorInfo(const ErrorInfo &copy);
		ErrorInfo(const std::string &filename, size_t line_number, size_t column_number);
		ErrorInfo	&operator=(const ErrorInfo &other);

		void	clear();

		const std::string	&filename() const;
		std::string			&filename();
		size_t				lineNumber() const;
		size_t				&lineNumber();
		size_t				columnNumber() const;
		size_t				&columnNumber();
};
