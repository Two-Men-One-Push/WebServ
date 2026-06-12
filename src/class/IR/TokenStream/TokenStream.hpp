#pragma once

#include "Token.hpp"
#include <iterator>

class	TokenStream
{
	private:
		std::vector<Token>	_tokens;
		std::string			_filename;
	public:
		TokenStream();
		~TokenStream();
		TokenStream(const TokenStream &copy);
		TokenStream	&operator=(const TokenStream &other);
	
		const std::vector<Token>	&getTokens() const;
		std::vector<Token>		&getTokensRef();
		const std::string		&getFilename() const;
		
		void	setFilename(const std::string &filename);
		void	addToken(const Token &token);

		typedef std::vector<Token>::iterator iterator;
		typedef std::vector<Token>::const_iterator const_iterator;

		iterator	begin();
		iterator	end();
		const_iterator	begin() const;
		const_iterator	end() const;
};