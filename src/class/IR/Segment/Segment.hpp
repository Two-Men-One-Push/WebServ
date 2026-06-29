#pragma once

#include "IR/ErrorInfo/ErrorInfo.hpp"
#include <string>

class	Segment: public ErrorInfo
{
	public:
		enum Type
		{
			NONE,
			DEFAULT,
			DEFAULT_ESCAPED,
			SQUOTE,
			SQUOTE_ESCAPED,
			DQUOTE,
			DQUOTE_ESCAPED,
		};
	private:
		Type		_type;
		std::string	_content;
		std::string	_raw_content;
	public:
		Segment();
		~Segment();
		Segment(const Segment &copy);
		Segment	&operator=(const Segment &other);

		Segment	&operator+=(const char c);
		Segment	&operator<<(const char c);

		void	clear();

		std::string	typeString() const;
		Type		type() const;
		Type		&type();

		const std::string	&content() const;
		const std::string	&rawContent() const;
};
