#include "analysis/Lexer/Lexer.hpp"
#include "IR/Segment/Segment.hpp"
#include "IR/Token/Token.hpp"
#include "IR/Word/Word.hpp"
#include <fstream>
#include <sstream>

enum	LexerState
{
	DEFAULT,
	DEFAULT_ESCAPE,
	SQUOTE,
	SQUOTE_ESCAPE,
	DQUOTE,
	DQUOTE_ESCAPE,
	COMMENT,
};

Lexer::Lexer()
{
}

Lexer::~Lexer()
{
}

TokenStream	Lexer::tokenize(const std::string &filename)
{
	TokenStream		token_stream(filename);
	std::ifstream	stream(filename.c_str());

	if (!stream.is_open())
		throw LexerFileOpenFailure("Failed to open file: ", filename);

	size_t		line_number = 1;
	size_t		column_number = 1;
	LexerState	state = DEFAULT;
	Word		word;
	Segment		segment;
	char		c;

	while (stream.get(c))
	{
		switch (state)
		{
			case DEFAULT:
				if (c == '#')
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
					token_stream.addToken(Token(Token::NEWLINE, filename, line_number, column_number));
					state = COMMENT;
				}
				else if (c == '{')
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
					token_stream.addToken(Token(Token::LBRACE, filename, line_number, column_number));
				}
				else if (c == '}')
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
					token_stream.addToken(Token(Token::RBRACE, filename, line_number, column_number));
				}
				else if (c == ';')
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
					token_stream.addToken(Token(Token::SEMICOLON, filename, line_number, column_number));
				}
				else if (c == '\n')
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
					token_stream.addToken(Token(Token::NEWLINE, filename, line_number, column_number));
				}
				else if (isspace(static_cast<unsigned char>(c)))
				{
					word += segment;
					if (!word.empty())
						token_stream.addToken(Token(word));
				}
				else if (c == '"')
				{
					word += segment;
					segment.filename() = filename;
					segment.lineNumber() = line_number;
					segment.columnNumber() = column_number;
					segment.type() = Segment::DQUOTE;
					segment << c;
					state = DQUOTE;
				}
				else if (c == '\'')
				{
					word += segment;
					segment.filename() = filename;
					segment.lineNumber() = line_number;
					segment.columnNumber() = column_number;
					segment.type() = Segment::SQUOTE;
					segment << c;
					state = SQUOTE;
				}
				else if (c == '\\')
				{
					word += segment;
					segment.filename() = filename;
					segment.lineNumber() = line_number;
					segment.columnNumber() = column_number;
					segment.type() = Segment::DEFAULT_ESCAPED;
					segment << c;
					state = DEFAULT_ESCAPE;
				}
				else if (isgraph(static_cast<unsigned char>(c)) || static_cast<unsigned char>(c) >= 128)
				{
					if (segment.type() == Segment::NONE)
					{
						segment.filename() = filename;
						segment.lineNumber() = line_number;
						segment.columnNumber() = column_number;
						segment.type() = Segment::DEFAULT;
					}
					segment += c;
					segment << c;
				}
				else
					throw LexerUnexpectedControlCharacter(filename, line_number, column_number);
				break;
			case SQUOTE:
				if (c == '\'')
				{
					segment << c;
					word += segment;
					state = DEFAULT;
				}
				else if (c == '\\')
				{
					word += segment;
					segment.filename() = filename;
					segment.lineNumber() = line_number;
					segment.columnNumber() = column_number;
					segment.type() = Segment::SQUOTE_ESCAPED;
					segment << c;
					state = SQUOTE_ESCAPE;
				}
				else
				{
					segment += c;
					segment << c;
				}
				break;
			case DQUOTE:
				if (c == '"')
				{
					segment << c;
					word += segment;
					state = DEFAULT;
				}
				else if (c == '\\')
				{
					word += segment;
					segment.filename() = filename;
					segment.lineNumber() = line_number;
					segment.columnNumber() = column_number;
					segment.type() = Segment::DQUOTE_ESCAPED;
					segment << c;
					state = DQUOTE_ESCAPE;
				}
				else
				{
					segment += c;
					segment << c;
				}
				break;
			case DEFAULT_ESCAPE:
				segment += c;
				segment << c;
				word += segment;
				segment.type() = Segment::DEFAULT;
				segment.filename() = filename;
				segment.lineNumber() = line_number;
				segment.columnNumber() = column_number + 1;
				state = DEFAULT;
				break;
			case SQUOTE_ESCAPE:
				segment += c;
				segment << c;
				word += segment;
				segment.type() = Segment::SQUOTE;
				segment.filename() = filename;
				segment.lineNumber() = line_number;
				segment.columnNumber() = column_number + 1;
				state = SQUOTE;
				break;
			case DQUOTE_ESCAPE:
				segment += c;
				segment << c;
				word += segment;
				segment.type() = Segment::DQUOTE;
				segment.filename() = filename;
				segment.lineNumber() = line_number;
				segment.columnNumber() = column_number + 1;
				state = DQUOTE;
				break;
			case COMMENT:
				if (c == '\n')
					state = DEFAULT;
				break;
			default:
				break;
		}
		if (c == '\n')
		{
			line_number++;
			column_number = 1;
		}
		else if ((static_cast<unsigned char>(c) & 0xC0) != 0x80)
			column_number++;
	}
	if (state == SQUOTE || state == DQUOTE)
		throw LexerUnexpectedEndOfFile("Unclosed quote", segment.filename(), segment.lineNumber(), segment.columnNumber());
	else if (state == DEFAULT_ESCAPE || state == SQUOTE_ESCAPE || state == DQUOTE_ESCAPE)
		throw LexerUnexpectedEndOfFile("Unexpected end of file after escape character", segment.filename(), segment.lineNumber(), segment.columnNumber());
	word += segment;
	if (!word.empty())
		token_stream.addToken(Token(word));
	token_stream.addToken(Token(Token::_EOF, filename, line_number, column_number));
	return token_stream;
}

Lexer::LexerFileOpenFailure::LexerFileOpenFailure(const std::string &description, const std::string &filename): _message()
{
	std::stringstream ss;
	ss << description << filename;
	_message = ss.str();
}

Lexer::LexerFileOpenFailure::~LexerFileOpenFailure() throw()
{
}

const char	*Lexer::LexerFileOpenFailure::what() const throw()
{
	return _message.c_str();
}

Lexer::LexerUnexpectedControlCharacter::LexerUnexpectedControlCharacter(const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << ": error: unexpected control character";
	_message = ss.str();
}

Lexer::LexerUnexpectedControlCharacter::~LexerUnexpectedControlCharacter() throw()
{
}

const char	*Lexer::LexerUnexpectedControlCharacter::what() const throw()
{
	return _message.c_str();
}

Lexer::LexerUnexpectedEndOfFile::LexerUnexpectedEndOfFile(const std::string &description, const std::string &filename, size_t line_number, size_t column_number): _message()
{
	std::stringstream ss;
	ss << filename << ":" << line_number << ":" << column_number << ": error: " << description;
	_message = ss.str();
}

Lexer::LexerUnexpectedEndOfFile::~LexerUnexpectedEndOfFile() throw()
{
}

const char	*Lexer::LexerUnexpectedEndOfFile::what() const throw()
{
	return _message.c_str();
}
