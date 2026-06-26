#pragma once

#include "Server.hpp"
#include "MimeTypes.hpp"
#include <vector>

#define DEFAULT_CLIENT_MAX_BODY_SIZE 1000000

class	Http
{
	private:
		size_t										_client_max_body_size;
		std::map<int, std::pair<int, std::string> >	_error_pages;
		std::map<std::string, std::string>			_cgi;
		MimeTypes									_types;
		std::vector<Server>							_servers;
	public:
		Http();
		~Http();
		Http(const Http &copy);
		Http	&operator=(const Http &other);

		const size_t										&clientMaxBodySize() const;
		size_t												&clientMaxBodySize();
		const std::map<int, std::pair<int, std::string> >	&errorPages() const;
		std::map<int, std::pair<int, std::string> >			&errorPages();
		const std::map<std::string, std::string>			&cgi() const;
		std::map<std::string, std::string>					&cgi();
		const MimeTypes										&types() const;
		MimeTypes											&types();
		const std::vector<Server>							&servers() const;
		std::vector<Server>									&servers();
};