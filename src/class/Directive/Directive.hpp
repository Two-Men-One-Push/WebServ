#pragma once

#include "Segment.hpp"
#include "Word.hpp"
#include <vector>
#include <list>
#include <string>

class	Directive
{
	private:
		Word					_name;
		std::vector<Word>		_args;
		std::list<Directive>	_children;
		std::string				_filename;
		size_t					_line_number;
		size_t					_column_number;
	public:
		~Directive();
		Directive(const Word &name, const std::string &filename, size_t line_number, size_t column_number);
		Directive(const Directive &copy);
		Directive	&operator=(const Directive &other);

		const Word					&getName() const;
		const std::vector<Word>		&getArgs() const;
		const std::list<Directive>	&getChildren() const;
		std::list<Directive>		&getChildrenRef();

		void				addArg(const Word &arg);
		void				addChild(const Directive &child);
		const std::string	&getFilename() const;
		size_t				getLineNumber() const;
		size_t				getColumnNumber() const;
};