#pragma once

#include "ErrorInfo.hpp"
#include "Segment.hpp"
#include "Word.hpp"
#include <vector>
#include <list>
#include <string>

class	Directive: public ErrorInfo
{
	private:
		Word					_name;
		std::vector<Word>		_args;
		bool					_has_body;
		std::list<Directive>	_children;
		ErrorInfo				_block_error_info;
	public:
		~Directive();
		Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number);
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);

		const Word					&getName() const;
		const std::vector<Word>		&getArgs() const;
		const std::list<Directive>	&getChildren() const;
		std::list<Directive>		&getChildrenRef();
		const ErrorInfo				&getBlockErrorInfo() const;
		bool						hasBody() const;

		void				addArg(const Word &arg);
		void				addChild(const Directive &child);
		void				setHasBody(bool has_body);
		void				setBlockErrorInfo(const ErrorInfo &error_info);
};