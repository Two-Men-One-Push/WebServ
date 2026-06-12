#pragma once

#include "Word.hpp"
#include <string>

class	Token: public ErrorInfo
{
	public:
		enum Type
		{
			NONE,
			WORD,
			LBRACE,
			RBRACE,
			SEMICOLON,
			NEWLINE,
			_EOF,
		};
	private:
		Type	_type;
		Word	_word;
	public:
		Token();
		~Token();
		Token(Type type, const std::string &filename, size_t line_number, size_t column_number);
		Token(Word &word);
		Token(const Token &copy);
		Token	&operator=(const Token &other);

		void	clear();

		std::string	typeString() const;
		Type		type() const;
		const Word	&word() const;
};
