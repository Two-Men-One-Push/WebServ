#include "TokenStream.hpp"

TokenStream::TokenStream(): _tokens()
{
}

TokenStream::~TokenStream()
{
}

TokenStream::TokenStream(const TokenStream &copy): _tokens(copy._tokens)
{
}

TokenStream	&TokenStream::operator=(const TokenStream &other)
{
	if (this != &other)
	{
		_tokens = other._tokens;
	}
	return *this;
}

const std::vector<Token>	&TokenStream::getTokens() const
{
	return _tokens;
}

std::vector<Token>	&TokenStream::getTokensRef()
{
	return _tokens;
}

const std::string	&TokenStream::getFilename() const
{
	return _filename;
}

void	TokenStream::setFilename(const std::string &filename)
{
	_filename = filename;
}

void	TokenStream::addToken(const Token &token)
{
	_tokens.push_back(token);
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