#include "URL.hpp"
#include "utils/parsing.hpp"
#include <cctype>

URL::URL(): _format(ERROR), _scheme(""), _user(""), _host(""), _port(-1), _path(), _rawQuery(""), _query(), _rawFragment(""), _fragment("")
{
}

URL::URL(const std::string &url): _format(ERROR), _scheme(""), _user(""), _host(""), _port(-1), _path(), _rawQuery(""), _query(), _rawFragment(""), _fragment("")
{
	if (url == "*")
	{
		this->_format = ASTERISK;
	}
	else if (url[0] == '/')
	{
		this->_format = ORIGIN;
		std::string origin = url;
		size_t fragment_pos = origin.find('#');
		if (fragment_pos != std::string::npos)
		{
			this->_rawFragment = '#' + origin.substr(fragment_pos + 1);
			if (decode(this->_fragment, origin.substr(fragment_pos + 1)))
			{
				this->_format = ERROR;
				return ;
			}
			origin.erase(fragment_pos);
		}
		size_t query_pos = origin.find('?');
		if (query_pos != std::string::npos)
		{
			std::string query_str = origin.substr(query_pos + 1);
			this->_rawQuery = '?' + query_str;
			origin.erase(query_pos);
			size_t start = 0;
			while (start < query_str.length())
			{
				size_t end = query_str.find('&', start);
				if (end == std::string::npos)
					end = query_str.length();
				std::string pair = query_str.substr(start, end - start);
				size_t equal_pos = pair.find('=');
				if (equal_pos != std::string::npos)
				{
					std::string key;
					if (decode(key, pair.substr(0, equal_pos)))
					{
						this->_format = ERROR;
						return ;
					}
					std::string value;
					if (decode(value, pair.substr(equal_pos + 1)))
					{
						this->_format = ERROR;
						return ;
					}
					this->_query[key] = value;
				}
				else
				{
					std::string key;
					if (decode(key, pair))
					{
						this->_format = ERROR;
						return ;
					}
					this->_query[key] = "";
				}
				start = end + 1;
			}
		}
		std::string	normalized_path;
		if (pathNormalize(normalized_path, origin))
		{
			this->_format = ERROR;
			return ;
		}
		normalized_path = trim_path(normalized_path);
		size_t	start = 0;
		while (start < normalized_path.length())
		{
			size_t	end = normalized_path.find('/', start);
			if (end == std::string::npos)
				end = normalized_path.length();
			std::string segment = normalized_path.substr(start, end - start);
			std::string decoded_segment;
			if (decode(decoded_segment, segment))
			{
				this->_format = ERROR;
				return ;
			}
			this->_path.push_back(decoded_segment);
			start = end + 1;
		}
	}
	else if (url.find("http://") == 0 || url.find("https://") == 0)
	{
		this->_format = ABSOLUTE;
		std::string	absolute = url;
		size_t scheme_end = absolute.find("://");
		if (scheme_end == std::string::npos)
		{
			this->_format = ERROR;
			return ;
		}
		this->_scheme = absolute.substr(0, scheme_end);
		absolute.erase(0, scheme_end + 3);
		size_t path_start;
		if (absolute.find('/') != std::string::npos)
			path_start = absolute.find('/');
		else if (absolute.find('?') != std::string::npos)
			path_start = absolute.find('?');
		else if (absolute.find('#') != std::string::npos)
			path_start = absolute.find('#');
		else
			path_start = absolute.length();
		std::string authority = absolute.substr(0, path_start);
		absolute.erase(0, path_start);
		size_t user_end = authority.find_last_of('@');
		if (user_end != std::string::npos)
		{
			if (decode(this->_user, authority.substr(0, user_end)))
			{
				this->_format = ERROR;
				return ;
			}
			authority.erase(0, user_end + 1);
		}
		size_t port_start = authority.find_last_of(':');
		if (port_start != std::string::npos)
		{
			if (decode(this->_host, authority.substr(0, port_start)))
			{
				this->_format = ERROR;
				return ;
			}
			std::string	port_str;
			if (decode(port_str, authority.substr(port_start + 1)))
			{
				this->_format = ERROR;
				return ;
			}
			if (!parseInt(port_str, this->_port))
			{
				this->_format = ERROR;
				return ;
			}
		}
		else
		{
			this->_host = authority;
		}
		std::string origin = absolute;
		size_t fragment_pos = origin.find('#');
		if (fragment_pos != std::string::npos)
		{
			this->_rawFragment = '#' + origin.substr(fragment_pos + 1);
			if (decode(this->_fragment, origin.substr(fragment_pos + 1)))
			{
				this->_format = ERROR;
				return ;
			}
			origin.erase(fragment_pos);
		}
		size_t query_pos = origin.find('?');
		if (query_pos != std::string::npos)
		{
			std::string query_str = origin.substr(query_pos + 1);
			this->_rawQuery = '?' + query_str;
			origin.erase(query_pos);
			size_t start = 0;
			while (start < query_str.length())
			{
				size_t end = query_str.find('&', start);
				if (end == std::string::npos)
					end = query_str.length();
				std::string pair = query_str.substr(start, end - start);
				size_t equal_pos = pair.find('=');
				if (equal_pos != std::string::npos)
				{
					std::string key;
					if (decode(key, pair.substr(0, equal_pos)))
					{
						this->_format = ERROR;
						return ;
					}
					std::string value;
					if (decode(value, pair.substr(equal_pos + 1)))
					{
						this->_format = ERROR;
						return ;
					}
					this->_query[key] = value;
				}
				else
				{
					std::string key;
					if (decode(key, pair))
					{
						this->_format = ERROR;
						return ;
					}
					this->_query[key] = "";
				}
				start = end + 1;
			}
		}
		if (origin.empty())
			return ;
		std::string	normalized_path;
		if (pathNormalize(normalized_path, origin))
		{
			this->_format = ERROR;
			return ;
		}
		normalized_path = trim_path(normalized_path);
		size_t	start = 0;
		while (start < normalized_path.length())
		{
			size_t	end = normalized_path.find('/', start);
			if (end == std::string::npos)
				end = normalized_path.length();
			std::string segment = normalized_path.substr(start, end - start);
			std::string decoded_segment;
			if (decode(decoded_segment, segment))
			{
				this->_format = ERROR;
				return ;
			}
			this->_path.push_back(decoded_segment);
			start = end + 1;
		}
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
		this->_format = other._format;
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
					return true;
				size_t LSB = std::string("0123456789abcdef").find(tolower(hex[1]));
				if (LSB == std::string::npos)
					return true;
				if (iscntrl(static_cast<char>(MSB + LSB)))
					return true;
				char c = static_cast<char>(MSB + LSB);
				output += c;
				i += 2;
			}
			else
			{
				return true;
			}
		}
		else
		{
			output += str[i];
		}
	}
	return false;
}

const std::string	URL::formatStr() const
{
	switch (this->_format)
	{
		case ABSOLUTE:
			return ("ABSOLUTE");
		case ORIGIN:
			return ("ORIGIN");
		case AUTHORITY:
			return ("AUTHORITY");
		case ASTERISK:
			return ("ASTERISK");
		default:
			return ("ERROR");
	}
	return ("ERROR");
}

const urlFormat	&URL::format() const
{
	return (this->_format);
}

urlFormat	&URL::format()
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

const int	&URL::port() const
{
	return (this->_port);
}

int	&URL::port()
{
	return (this->_port);
}

const std::vector<std::string>	&URL::path() const
{
	return (this->_path);
}

std::vector<std::string>	&URL::path()
{
	return (this->_path);
}

const std::string	&URL::rawQuery() const
{
	return (this->_rawQuery);
}

std::string	&URL::rawQuery()
{
	return (this->_rawQuery);
}

const std::map<std::string, std::string>	&URL::query() const
{
	return (this->_query);
}

std::map<std::string, std::string>	&URL::query()
{
	return (this->_query);
}

const std::string	&URL::rawFragment() const
{
	return (this->_rawFragment);
}

std::string	&URL::rawFragment()
{
	return (this->_rawFragment);
}

const std::string	&URL::fragment() const
{
	return (this->_fragment);
}

std::string	&URL::fragment()
{
	return (this->_fragment);
}