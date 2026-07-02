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
		ErrorInfo::operator=(other);
		_type = other._type;
		_word = other._word;
	}
	return *this;
}

void	Token::clear()
{
	ErrorInfo::clear();
	_type = NONE;
	_word.clear();
}

std::string	Token::typeString() const
{
	switch (_type)
	{
		case NONE:      return "NONE";
		case WORD:      return "WORD";
		case LBRACE:    return "LBRACE";
		case RBRACE:    return "RBRACE";
		case SEMICOLON: return "SEMICOLON";
		case NEWLINE:   return "NEWLINE";
		case _EOF:      return "EOF";
		default:        return "UNKNOWN";
	}
}

Token::Type	Token::type() const
{
	return _type;
}

const Word	&Token::word() const
{
	return _word;
}
