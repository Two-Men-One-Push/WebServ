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
		std::vector<std::string>			_path;
		std::string							_rawQuery;
		std::map<std::string, std::string>	_query;
		std::string							_rawFragment;
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
		const std::vector<std::string>				&path() const;
		std::vector<std::string>					&path();
		const std::string							&rawQuery() const;
		std::string									&rawQuery();
		const std::map<std::string, std::string>	&query() const;
		std::map<std::string, std::string>			&query();
		const std::string							&rawFragment() const;
		std::string									&rawFragment();
		const std::string							&fragment() const;
		std::string									&fragment();
};