#include "TokenStream.hpp"

TokenStream::TokenStream(): _tokens(), _filename()
{
}

TokenStream::~TokenStream()
{
}

TokenStream::TokenStream(const TokenStream &copy): _tokens(copy._tokens), _filename(copy._filename)
{
}

TokenStream	&TokenStream::operator=(const TokenStream &other)
{
	if (this != &other)
	{
		_tokens = other._tokens;
		_filename = other._filename;
	}
	return *this;
}

TokenStream::iterator	TokenStream::begin()
{
	return _tokens.begin();
}

TokenStream::iterator	TokenStream::end()
{
	return _tokens.end();
}

TokenStream::const_iterator	TokenStream::begin() const
{
	return _tokens.begin();
}

TokenStream::const_iterator	TokenStream::end() const
{
	return _tokens.end();
}

const std::vector<Token>	&TokenStream::tokens() const
{
	return _tokens;
}

std::vector<Token>	&TokenStream::tokens()
{
	return _tokens;
}

const std::string	&TokenStream::filename() const
{
	return _filename;
}

std::string	&TokenStream::filename()
{
	return _filename;
}

void	TokenStream::addToken(const Token &token)
{
	_tokens.push_back(token);
}
