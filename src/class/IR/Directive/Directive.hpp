#pragma once

#include "ErrorInfo.hpp"
#include "Word.hpp"
#include <list>
#include <vector>

class	Directive: public ErrorInfo
{
	private:
		Word					_name;
		std::vector<Word>		_args;
		bool					_has_body;
		ErrorInfo				_block_error_info;
		std::list<Directive>	_children;
	public:
		Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number);
		~Directive();
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);

		const Word					&name() const;
		const std::vector<Word>		&args() const;
		bool						hasBody() const;
		const ErrorInfo				&blockErrorInfo() const;
		const std::list<Directive>	&children() const;
		std::list<Directive>		&children();

		void	addArg(const Word &arg);
		void	addChild(const Directive &child);
		void	setHasBody(bool has_body);
		void	setBlockErrorInfo(const ErrorInfo &error_info);
};
