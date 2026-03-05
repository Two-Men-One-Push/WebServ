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

LocationContext::~LocationContext()
{}

LocationContext::LocationContext():
_path(),
_methods(),
_root(),
_index(),
_autoindex(),
_redirection(),
_cgi_info()
{}

LocationContext::LocationContext(const LocationContext &copy):
_path(copy._path),
_methods(copy._methods),
_root(copy._root),
_index(copy._index),
_autoindex(copy._autoindex),
_redirection(copy._redirection),
_cgi_info(copy._cgi_info)
{}

LocationContext	&LocationContext::operator=(const LocationContext &other)
{
	if (this != &other)
	{
		this->_path = other._path;
		this->_methods = other._methods;
		this->_root = other._root;
		this->_index = other._index;
		this->_autoindex = other._autoindex;
		this->_redirection = other._redirection;
		this->_cgi_info = other._cgi_info;
	}
	return *this;
}

LocationContext::LocationContext(std::istream &input)
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