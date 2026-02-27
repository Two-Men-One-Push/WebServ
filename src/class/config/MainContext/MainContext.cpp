#include "MainContext.hpp"
#include <cctype>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

enum	e_state
{
	DEFAULT		= 0,
	STRING		= 1 << 0,
	BLOCK		= 1 << 1,
	WAIT_BLOCK	= 1 << 2,
	LITTERAL	= 1 << 3,
};

MainContext::~MainContext()
{}

MainContext::MainContext(): _servers()
{}

MainContext::MainContext(const MainContext &copy): _servers(copy._servers)
{}

MainContext	&MainContext::operator=(const MainContext &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
	}
	return *this;
}

MainContext::MainContext(std::istream &input)
{
	std::stringstream	tmp;
	std::string			word;
	int					state;
	int					scope;
	char				c;

	scope = 0;
	state = DEFAULT;
	while (input.get(c))
	{
		if (state & WAIT_BLOCK)
		{
			if (c == '{')
			{
				state = BLOCK;
				scope++;
			}
			else if (!isspace(c))
			{
				throw std::invalid_argument("Invalid Config");
			}
		}
		else if (state & BLOCK)
		{
			if (state & STRING)
			{
				if (!(state & LITTERAL))
				{
					if (c == '\"' || c == '\'')
						state &= ~STRING;
					else if (c == '\\')
						state |= LITTERAL;
				}
			}
			else if (c == '\"' || c == '\'')
				state |= STRING;
			else if (c == '{')
				scope++;
			else if (c == '}')
				scope--;
			else if (c == '\\')
				state |= LITTERAL;
			if (scope <= 0)
				state &= ~BLOCK;
			else
				tmp << c;
		}
		else
		{
			if (!isspace(c))
				word += c;
			if (word == "server")
			{
				state = WAIT_BLOCK;
				word.clear();
			}
			else if (isspace(c) && !word.empty() && !(state & BLOCK))
			{
				throw std::invalid_argument("Invalid Config");
			}
		}
	}
	std::cout << "[" << tmp.rdbuf() << "]" << std::endl;
}