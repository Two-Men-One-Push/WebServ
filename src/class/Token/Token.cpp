#include "Token.hpp"

Token::Token(): ErrorInfo(), _type(NONE), _word()
{
}

Token::~Token()
{
}

Token::Token(Type type, const std::string &filename, size_t line_number, size_t column_number): ErrorInfo(filename, line_number, column_number), _type(type), _word()
{
}

Token::Token(Word &word): ErrorInfo(word), _type(WORD), _word(word)
{
	word.clear();
}

Token::Token(const Token &copy): ErrorInfo(copy), _type(copy._type), _word(copy._word)
{
}

Token	&Token::operator=(const Token &other)
{
	if (this != &other)
	{
		_type = other._type;
		_word = other._word;
		ErrorInfo::operator=(other);
	}
	return *this;
}

void	Token::clear()
{
	_type = NONE;
	_word.clear();
	ErrorInfo::clear();
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

void	Token::setWord(const Word &word)
{
	_word = word;
}

void	Token::setType(Type type)
{
	_type = type;
}