#pragma once

#include "IR/Token/Token.hpp"
#include <vector>

class	TokenStream: public ErrorInfo
{
	private:
		std::vector<Token>	_tokens;
	public:
		TokenStream(std::string filename);
		~TokenStream();
		TokenStream(const TokenStream &copy);
		TokenStream	&operator=(const TokenStream &other);

		typedef std::vector<Token>::iterator		iterator;
		typedef std::vector<Token>::const_iterator	const_iterator;

		iterator		begin();
		iterator		end();
		const_iterator	begin() const;
		const_iterator	end() const;

		const std::vector<Token>	&tokens() const;
		std::vector<Token>			&tokens();

		void	addToken(const Token &token);
};
