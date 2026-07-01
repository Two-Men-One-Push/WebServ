#pragma once

#include "IR/TokenStream/TokenStream.hpp"
#include <exception>

class	Lexer
{
	private:
		Lexer();
	public:
		~Lexer();
		static TokenStream	tokenize(const std::string &filename);

		class	LexerFileOpenFailure: public std::exception
		{
			private:
				std::string		_message;
			public:
				virtual ~LexerFileOpenFailure() throw();
				LexerFileOpenFailure(const std::string &description, const std::string &filename);
				virtual const char	*what() const throw();
		};
		class	LexerUnexpectedControlCharacter: public std::exception
		{
			private:
				std::string		_message;
			public:
				virtual ~LexerUnexpectedControlCharacter() throw();
				LexerUnexpectedControlCharacter(const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
		class	LexerUnexpectedEndOfFile: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~LexerUnexpectedEndOfFile() throw();
				LexerUnexpectedEndOfFile(const std::string &description, const std::string &filename, size_t line_number, size_t column_number);
				virtual const char	*what() const throw();
		};
};