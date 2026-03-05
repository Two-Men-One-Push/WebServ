#include "MainContext.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

enum	e_state
{
	DEFAULT			= 0,
	LITTERAL		= 1 << 1,
	SQ_STRING		= 1 << 2,
	DQ_STRING		= 1 << 3,
	LITTERAL_STRING	= 1 << 4,
	SERVER_BLOCK	= 1 << 5,
	SERVER			= 1 << 6,
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
		if (state & LITTERAL)
		{
			word += c;
			state &= ~LITTERAL;
		}
		else if (c == '\\')
		{
			state |= LITTERAL;
			if (state & LITTERAL_STRING)
				word += c;
		}
		else if (state & SQ_STRING)
		{
			if (c == '\'')
				state &= ~SQ_STRING;
			else
				word += c;
		}
		else if (c == '\'')
		{
			state |= SQ_STRING;
			if (state & LITTERAL_STRING)
				word += c;
		}
		else if (state & DQ_STRING)
		{
			if (c == '\"')
				state &= ~DQ_STRING;
			if (state & LITTERAL_STRING)
				word += c;
		}
		else if (c == '\"')
		{
			state |= DQ_STRING;
			if (state & LITTERAL_STRING)
				word += c;
		}
		else if (state & SERVER)
		{
			if (c == '{')
			{
				state = SERVER_BLOCK | LITTERAL_STRING;
				scope++;
			}
			else if (!isspace(c))
				throw std::invalid_argument("Invalid Config");
		}
		else if (state & SERVER_BLOCK)
		{
			if (c == '{')
				scope++;
			else if (c == '}')
				scope--;
			if (scope <= 0)
			{
				state = DEFAULT;
				this->_servers.push_back(ServerContext(tmp));
			}
			else
			{
				word += c;
				tmp << word;
				word.clear();
			}
		}
		else
		{
			if (!isspace(c))
				word += c;
			else if (word == "server")
			{
				state = SERVER | LITTERAL_STRING;
				std::cout << "[" << word << "]" << std::endl;
				word.clear();
			}
			else if (!word.empty())
				throw std::invalid_argument("Invalid Config");
		}
	}
	if (scope > 0)
		throw std::invalid_argument("Invalid Config");
}