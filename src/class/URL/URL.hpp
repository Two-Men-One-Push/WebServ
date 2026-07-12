#pragma once

#include <string>
#include <map>
#include <vector>

typedef enum {URL_ABSOLUTE, URL_ORIGIN, URL_AUTHORITY, URL_ASTERISK, URL_ERROR} urlFormat;

class URL
{
	private:
		urlFormat							_format;
		std::string							_scheme;
		std::string							_user;
		std::string							_host;
		int									_port;
		std::string							_path;
		std::vector<std::string>			_segments;
		std::string							_queryString;
		std::map<std::string, std::string>	_query;
		std::string							_fragmentString;
		std::string							_fragment;
	public:
		URL();
		URL(const std::string &uri);
		~URL();
		URL(const URL &copy);
		URL	&operator=(const URL &other);

		static bool	decode(std::string &output, const std::string &str);

		const std::string	formatStr() const;

		const urlFormat								&format() const;
		urlFormat									&format();
		const std::string							&scheme() const;
		std::string									&scheme();
		const std::string							&user() const;
		std::string									&user();
		const std::string							&host() const;
		std::string									&host();
		const int									&port() const;
		int											&port();
		const std::string							&path() const;
		std::string									&path();
		const std::vector<std::string>				&segments() const;
		std::vector<std::string>					&segments();
		const std::string							&queryString() const;
		std::string									&queryString();
		const std::map<std::string, std::string>	&query() const;
		std::map<std::string, std::string>			&query();
		const std::string							&fragmentString() const;
		std::string									&fragmentString();
		const std::string							&fragment() const;
		std::string									&fragment();
};