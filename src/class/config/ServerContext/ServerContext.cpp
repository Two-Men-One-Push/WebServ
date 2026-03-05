#include "ServerContext.hpp"
#include "LocationContext.hpp"
#include <iostream>
#include <sstream>

enum	e_state
{
	DEFAULT			= 0,
	LITTERAL		= 1 << 1,
	SQ_STRING		= 1 << 2,
	DQ_STRING		= 1 << 3,
	LITTERAL_STRING	= 1 << 4,
	LOCATION_BLOCK	= 1 << 5,
	LOCATION_URI	= 1 << 6,
	LOCATION		= 1 << 7,
};

ServerContext::~ServerContext()
{}

ServerContext::ServerContext():
_port(),
_host(),
_server_names(),
_error_pages(),
_client_max_body_size(),
_locations()
{}

ServerContext::ServerContext(const ServerContext &copy):
_port(copy._port),
_host(copy._host),
_server_names(copy._server_names),
_error_pages(copy._error_pages),
_client_max_body_size(copy._client_max_body_size),
_locations(copy._locations)
{}

ServerContext &ServerContext::operator=(const ServerContext &other)
{
	if (this != &other)
	{
		this->_port = other._port;
		this->_host = other._host;
		this->_server_names = other._server_names;
		this->_error_pages = other._error_pages;
		this->_client_max_body_size = other._client_max_body_size;
		this->_locations = other._locations;
	}
	return *this;
}

ServerContext::ServerContext(std::istream &input)
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
			state &= ~LITTERAL;
			word += c;
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
		else if (state & LOCATION_URI)
		{
			if (!isspace(c))
				word += c;
			else if (!word.empty())
			{
				state = LOCATION | LITTERAL_STRING;
				std::cout << "[" << word << "]" << std::endl;
				word.clear();
			}
		}
		else if (state & LOCATION)
		{
			if (c == '{')
			{
				state = LOCATION_BLOCK | LITTERAL_STRING;
				scope++;
			}
			else if (!isspace(c))
				throw std::invalid_argument("Invalid Config1");
		}
		else if (state & LOCATION_BLOCK)
		{
			if (c == '{')
				scope++;
			else if (c == '}')
				scope--;
			if (scope <= 0)
			{
				state = DEFAULT;
				this->_locations.push_back(LocationContext(tmp));
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
			else if (word == "location")
			{
				state = LOCATION_URI;
				std::cout << "[" << word << "]" << std::endl;
				word.clear();
			}
			else if (!word.empty())
			{
				throw std::invalid_argument("Invalid Config3");
			}
		}
	}
	if (scope > 0)
		throw std::invalid_argument("Invalid Config4");
}