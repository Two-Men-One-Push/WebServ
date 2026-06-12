#pragma once

#include "ErrorInfo.hpp"
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
	
		const std::string	getTypeString() const;

		Type				getType() const;
		const std::string	&getContent() const;
		const std::string	&getRawContent() const;
	
		void	setType(Type type);
		void	setContent(const std::string &content);
		void	setRawContent(const std::string &raw_content);
};