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

		void	setFilename(const std::string &filename);
		void	setLineNumber(size_t line_number);
		void	setColumnNumber(size_t column_number);

		const std::string	&getFilename() const;
		size_t				getLineNumber() const;
		size_t				getColumnNumber() const;
};