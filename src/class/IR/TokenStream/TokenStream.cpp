#include "TokenStream.hpp"

TokenStream::TokenStream(std::string filename): ErrorInfo(filename, 1, 1), _tokens()
{
}

TokenStream::~TokenStream()
{
}

TokenStream::TokenStream(const TokenStream &copy): ErrorInfo(copy), _tokens(copy._tokens)
{
}

TokenStream	&TokenStream::operator=(const TokenStream &other)
{
	if (this != &other)
	{
		ErrorInfo::operator=(other);
		_tokens = other._tokens;
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

void	TokenStream::addToken(const Token &token)
{
	_tokens.push_back(token);
}
