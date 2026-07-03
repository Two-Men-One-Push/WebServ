#include "URL.hpp"
#include <iostream>
#include <vector>

URL::URL(): _path(""), _query()
{
}

//	scheme://user@host:port/path?query#fragment
//	/path?query#fragment
//	host:port
//	*
URL::URL(const std::string &url): _scheme(""), _user(""), _host(""), _port(""), _path(""), _query(), _fragment("")
{
	if (url == "*")
	{
		this->_format = ASTERISK;
		return ;
	}
	else if (url[0] == '/')
	{
		this->_format = ORIGIN;
		
	}
	else if (url.find("http://") == 0 || url.find("https://") == 0)
	{
		this->_format = ABSOLUTE;
	}
	else
	{
		this->_format = AUTHORITY;
	}
}

URL::~URL()
{
}

URL::URL(const URL &copy): _scheme(copy._scheme), _user(copy._user), _host(copy._host), _port(copy._port), _path(copy._path), _query(copy._query), _fragment(copy._fragment)
{
}

URL	&URL::operator=(const URL &other)
{
	if (this != &other)
	{
		this->_scheme = other._scheme;
		this->_user = other._user;
		this->_host = other._host;
		this->_port = other._port;
		this->_path = other._path;
		this->_query = other._query;
		this->_fragment = other._fragment;
	}
	return (*this);
}

bool	URL::decode(std::string &output, const std::string &str)
{
	output.clear();
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == '%')
		{
			if (i + 2 < str.length())
			{
				std::string hex = str.substr(i + 1, 2);
				
				size_t MSB = std::string("0123456789abcdef").find(tolower(hex[0])) * 16;
				if (MSB == std::string::npos)
					return false;
				size_t LSB = std::string("0123456789abcdef").find(tolower(hex[1]));
				if (LSB == std::string::npos)
					return false;
				char c = static_cast<char>(MSB + LSB);
				output += c;
				i += 2;
			}
			else
			{
				return false;
			}
		}
		else if (str[i] == '+')
		{
			output += ' ';
		}
		else
		{
			output += str[i];
		}
	}
	return true;
}

const URL::urlFormat	&URL::format() const
{
	return (this->_format);
}

URL::urlFormat	&URL::format()
{
	return (this->_format);
}

const std::string	&URL::scheme() const
{
	return (this->_scheme);
}

std::string	&URL::scheme()
{
	return (this->_scheme);
}

const std::string	&URL::user() const
{
	return (this->_user);
}

std::string	&URL::user()
{
	return (this->_user);
}

const std::string	&URL::host() const
{
	return (this->_host);
}

std::string	&URL::host()
{
	return (this->_host);
}

const std::string	&URL::port() const
{
	return (this->_port);
}

std::string	&URL::port()
{
	return (this->_port);
}

const std::string	&URL::path() const
{
	return (this->_path);
}

std::string	&URL::path()
{
	return (this->_path);
}

const std::map<std::string, std::string>	&URL::query() const
{
	return (this->_query);
}

std::map<std::string, std::string>	&URL::query()
{
	return (this->_query);
}

const std::string	&URL::fragment() const
{
	return (this->_fragment);
}

std::string	&URL::fragment()
{
	return (this->_fragment);
}