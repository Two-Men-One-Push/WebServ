#pragma once

#include <string>
#include <map>

class URL
{
	private:
		typedef enum {ABSOLUTE, ORIGIN, AUTHORITY, ASTERISK} urlFormat;
		urlFormat	_format;
		std::string	_scheme;
		std::string	_user;
		std::string	_host;
		std::string	_port;
		std::string	_path;
		std::map<std::string, std::string>	_query;
		std::string	_fragment;
	public:
		URL();
		URL(const std::string &uri);
		~URL();
		URL(const URL &copy);
		URL	&operator=(const URL &other);

		static bool	decode(std::string &output, const std::string &str);

		const urlFormat		&format() const;
		urlFormat			&format();
		const std::string	&scheme() const;
		std::string			&scheme();
		const std::string	&user() const;
		std::string			&user();
		const std::string	&host() const;
		std::string			&host();
		const std::string	&port() const;
		std::string			&port();
		const std::string	&path() const;
		std::string			&path();
		const std::map<std::string, std::string>	&query() const;
		std::map<std::string, std::string>			&query();
		const std::string	&fragment() const;
		std::string			&fragment();
};