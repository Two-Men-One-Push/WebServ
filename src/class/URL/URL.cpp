#include "URL.hpp"
#include "utils/parsing.hpp"
#include <cctype>
#include <string>

URL::URL():
_format(URL_ERROR),
_raw(""),
_scheme(""),
_user(""),
_host(""),
_port(-1),
_rawSegments(),
_segments(),
_queryString(""),
_query(),
_fragmentString(""),
_fragment(""),
_folder(false)
{
}

URL::URL(const std::string &url):
_format(URL_ERROR),
_raw(url),
_scheme(""),
_user(""),
_host(""),
_port(-1),
_rawSegments(),
_segments(),
_queryString(""),
_query(),
_fragmentString(""),
_fragment(""),
_folder(false)
{
	if (url == "*")
	{
		this->_format = URL_ASTERISK;
	}
	else if (url[0] == '/')
	{
		this->_format = URL_ORIGIN;
		std::string origin = url;
		size_t fragment_pos = origin.find('#');
		if (fragment_pos != std::string::npos)
		{
			this->_fragmentString = origin.substr(fragment_pos + 1);
			if (decode(this->_fragment, origin.substr(fragment_pos + 1)))
			{
				this->_format = URL_ERROR;
				return ;
			}
			origin.erase(fragment_pos);
		}
		size_t query_pos = origin.find('?');
		if (query_pos != std::string::npos)
		{
			this->_queryString = origin.substr(query_pos + 1);
			std::string query_str = origin.substr(query_pos + 1);
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
						this->_format = URL_ERROR;
						return ;
					}
					std::string value;
					if (decode(value, pair.substr(equal_pos + 1)))
					{
						this->_format = URL_ERROR;
						return ;
					}
					this->_query[key] = value;
				}
				else
				{
					std::string key;
					if (decode(key, pair))
					{
						this->_format = URL_ERROR;
						return ;
					}
					this->_query[key] = "";
				}
				start = end + 1;
			}
		}
		if (origin.empty())
		{
			this->_folder = true;
			return ;
		}
		if (origin[origin.length() - 1] == '/')
			this->_folder = true;
		std::string	decoded_origin;
		if (decode(decoded_origin, origin))
		{
			this->_format = URL_ERROR;
			return ;
		}
		std::string	normalized_path;
		if (pathNormalize(normalized_path, decoded_origin))
		{
			this->_format = URL_ERROR;
			return ;
		}
		size_t	start = 0;
		while (start < normalized_path.length())
		{
			size_t	end = normalized_path.find('/', start);
			if (end == std::string::npos)
				end = normalized_path.length();
			std::string	segment = normalized_path.substr(start, end - start);
			if (!segment.empty())
				this->_normalizedSegments.push_back(segment);
			start = end + 1;
		}
		std::string path = trim_path(origin);
		start = 0;
		while (start < path.length())
		{
			size_t	end = path.find('/', start);
			if (end == std::string::npos)
				end = path.length();
			std::string	segment = path.substr(start, end - start);
			std::string	decoded_segment;
			if (decode(decoded_segment, segment))
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (!segment.empty() && !decoded_segment.empty())
			{
				this->_rawSegments.push_back(segment);
				this->_segments.push_back(decoded_segment);
			}
			start = end + 1;
		}
	}
	else if (url.find("http://") == 0 || url.find("https://") == 0)
	{
		this->_format = URL_ABSOLUTE;
		std::string	absolute = url;
		size_t scheme_end = absolute.find("://");
		if (scheme_end == std::string::npos)
		{
			this->_format = URL_ERROR;
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
				this->_format = URL_ERROR;
				return ;
			}
			authority.erase(0, user_end + 1);
		}
		size_t port_start = authority.find_last_of(':');
		if (port_start != std::string::npos)
		{
			if (port_start == 0)
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (decode(this->_host, authority.substr(0, port_start)))
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (port_start + 1 >= authority.length())
			{
				this->_format = URL_ERROR;
				return ;
			}
			std::string	port_str;
			if (decode(port_str, authority.substr(port_start + 1)))
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (!parseInt(port_str, this->_port))
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (this->_port < 0 || this->_port > 65535)
			{
				this->_format = URL_ERROR;
				return ;
			}
		}
		else
		{
			if (authority.empty())
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (decode(this->_host, authority))
			{
				this->_format = URL_ERROR;
				return ;
			}
		}
		std::string origin = absolute;
		size_t fragment_pos = origin.find('#');
		if (fragment_pos != std::string::npos)
		{
			this->_fragmentString = origin.substr(fragment_pos + 1);
			if (decode(this->_fragment, origin.substr(fragment_pos + 1)))
			{
				this->_format = URL_ERROR;
				return ;
			}
			origin.erase(fragment_pos);
		}
		size_t query_pos = origin.find('?');
		if (query_pos != std::string::npos)
		{
			this->_queryString = origin.substr(query_pos + 1);
			std::string query_str = origin.substr(query_pos + 1);
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
						this->_format = URL_ERROR;
						return ;
					}
					std::string value;
					if (decode(value, pair.substr(equal_pos + 1)))
					{
						this->_format = URL_ERROR;
						return ;
					}
					this->_query[key] = value;
				}
				else
				{
					std::string key;
					if (decode(key, pair))
					{
						this->_format = URL_ERROR;
						return ;
					}
					this->_query[key] = "";
				}
				start = end + 1;
			}
		}
		if (origin.empty())
		{
			this->_folder = true;
			return ;
		}
		if (origin[origin.length() - 1] == '/')
			this->_folder = true;
		std::string	decoded_origin;
		if (decode(decoded_origin, origin))
		{
			this->_format = URL_ERROR;
			return ;
		}
		std::string	normalized_path;
		if (pathNormalize(normalized_path, decoded_origin))
		{
			this->_format = URL_ERROR;
			return ;
		}
		size_t	start = 0;
		while (start < normalized_path.length())
		{
			size_t	end = normalized_path.find('/', start);
			if (end == std::string::npos)
				end = normalized_path.length();
			std::string	segment = normalized_path.substr(start, end - start);
			if (!segment.empty())
				this->_normalizedSegments.push_back(segment);
			start = end + 1;
		}
		std::string path = trim_path(origin);
		start = 0;
		while (start < path.length())
		{
			size_t	end = path.find('/', start);
			if (end == std::string::npos)
				end = path.length();
			std::string	segment = path.substr(start, end - start);
			std::string	decoded_segment;
			if (decode(decoded_segment, segment))
			{
				this->_format = URL_ERROR;
				return ;
			}
			if (!segment.empty() && !decoded_segment.empty())
			{
				this->_rawSegments.push_back(segment);
				this->_segments.push_back(decoded_segment);
			}
			start = end + 1;
		}
	}
	else
	{
		this->_format = URL_AUTHORITY;
	}
}

URL::~URL()
{
}

URL::URL(const URL &copy):
_format(copy._format),
_raw(copy._raw),
_scheme(copy._scheme),
_user(copy._user),
_host(copy._host),
_port(copy._port),
_rawSegments(copy._rawSegments),
_segments(copy._segments),
_normalizedSegments(copy._normalizedSegments),
_queryString(copy._queryString),
_query(copy._query),
_fragmentString(copy._fragmentString),
_fragment(copy._fragment),
_folder(copy._folder)
{}

URL	&URL::operator=(const URL &other)
{
	if (this != &other)
	{
		this->_format = other._format;
		this->_raw = other._raw;
		this->_scheme = other._scheme;
		this->_user = other._user;
		this->_host = other._host;
		this->_port = other._port;
		this->_rawSegments = other._rawSegments;
		this->_segments = other._segments;
		this->_normalizedSegments = other._normalizedSegments;
		this->_queryString = other._queryString;
		this->_query = other._query;
		this->_fragmentString = other._fragmentString;
		this->_fragment = other._fragment;
		this->_folder = other._folder;
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
				if (static_cast<char>(MSB + LSB) == '/')
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
		case URL_ABSOLUTE:
			return ("URL_ABSOLUTE");
		case URL_ORIGIN:
			return ("URL_ORIGIN");
		case URL_AUTHORITY:
			return ("URL_AUTHORITY");
		case URL_ASTERISK:
			return ("URL_ASTERISK");
		default:
			return ("URL_ERROR");
	}
	return ("URL_ERROR");
}

const urlFormat	&URL::format() const
{
	return (this->_format);
}

urlFormat	&URL::format()
{
	return (this->_format);
}

const std::string	&URL::raw() const
{
	return (this->_raw);
}

std::string	&URL::raw()
{
	return (this->_raw);
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

const std::vector<std::string>	&URL::rawSegments() const
{
	return (this->_rawSegments);
}

std::vector<std::string>	&URL::rawSegments()
{
	return (this->_rawSegments);
}

const std::vector<std::string>	&URL::segments() const
{
	return (this->_segments);
}

std::vector<std::string>	&URL::segments()
{
	return (this->_segments);
}

const std::vector<std::string>	&URL::normalizedSegments() const
{
	return (this->_normalizedSegments);
}

std::vector<std::string>	&URL::normalizedSegments()
{
	return (this->_normalizedSegments);
}

const std::string	&URL::queryString() const
{
	return (this->_queryString);
}

std::string	&URL::queryString()
{
	return (this->_queryString);
}

const std::map<std::string, std::string>	&URL::query() const
{
	return (this->_query);
}

std::map<std::string, std::string>	&URL::query()
{
	return (this->_query);
}

const std::string	&URL::fragmentString() const
{
	return (this->_fragmentString);
}

std::string	&URL::fragmentString()
{
	return (this->_fragmentString);
}

const std::string	&URL::fragment() const
{
	return (this->_fragment);
}

std::string	&URL::fragment()
{
	return (this->_fragment);
}
const bool	&URL::folder() const
{
	return (this->_folder);
}

bool	&URL::folder()
{
	return (this->_folder);
}