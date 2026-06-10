#include "Token.hpp"

Token::Token(): _type(NONE), _word(), _filename(), _line_number(0), _column_number(0)
{
}

Token::~Token()
{
}

Token::Token(Type type, const std::string &filename, size_t line_number, size_t column_number): _type(type), _word(), _filename(filename), _line_number(line_number), _column_number(column_number)
{
}

Token::Token(Word &word): _type(WORD), _word(word), _filename(word.getFilename()), _line_number(word.getLineNumber()), _column_number(word.getColumnNumber())
{
	word.clear();
}

Token::Token(const Token &copy): _type(copy._type), _word(copy._word), _filename(copy._filename), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

Token	&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_type = other._type;
		_word = other._word;
		_filename = other._filename;
		_line_number = other._line_number;
		_column_number = other._column_number;
	}
	return *this;
}

void	Token::clear()
{
	_type = NONE;
	_word.clear();
	_filename.clear();
	_line_number = 0;
	_column_number = 0;
}

const std::string	Token::getTypeString() const
{
	switch (_type)
	{
		case NONE:
			return "NONE";
		case WORD:
			return "WORD";
		case LBRACE:
			return "LBRACE";
		case RBRACE:
			return "RBRACE";
		case SEMICOLON:
			return "SEMICOLON";
		case NEWLINE:
			return "NEWLINE";
		case _EOF:
			return "EOF";
		default:
			return "UNKNOWN";
	}
}

Token::Type	Token::getType() const
{
	return _type;
}

const Word	&Token::getWord() const
{
	return _word;
}

const std::string	&Token::getFilename() const
{
	return _filename;
}

size_t	Token::getLineNumber() const
{
	return _line_number;
}

size_t	Token::getColumnNumber() const
{
	return _column_number;
}

void	Token::setFilename(const std::string &filename)
{
	_filename = filename;
}

void	Token::setWord(const Word &word)
{
	_word = word;
}

void	Token::setType(Type type)
{
	_type = type;
}

void	Token::setLineNumber(size_t line_number)
{
	_line_number = line_number;
}

void	Token::setColumnNumber(size_t column_number)
{
	_column_number = column_number;
}