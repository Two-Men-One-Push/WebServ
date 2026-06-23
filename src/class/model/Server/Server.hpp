#pragma once

#include "Location.hpp"
#include <vector>
#include <string>

class	Server
{
	private:
		std::vector<std::string>	_listen;
		std::vector<std::string>	_server_names;
		std::string					_root;
		std::vector<std::string>	_index_files;
		std::map<int, std::string>	_error_pages;
		size_t						_client_max_body_size;
		MimeType					_mimeType;
		std::vector<Location>		_locations;
	public:
		Server();
		~Server();
		Server(const Server &copy);
		Server	&operator=(const Server &other);

		const std::vector<std::string>	&listen() const;
		std::vector<std::string>		&listen();
		const std::vector<Location>		&locations() const;
		std::vector<Location>			&locations();
		const MimeType					&mimetype() const;
		MimeType						&mimetype();
};