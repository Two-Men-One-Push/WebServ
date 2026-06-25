#pragma once

#include "Http.hpp"
#include "MimeTypes.hpp"
#include "Location.hpp"
#include <vector>
#include <string>

class	Server
{
	private:
		std::vector<int>							_listen;
		std::vector<std::string>					_server_names;
		std::string									_root;
		std::vector<std::string>					_index_files;
		std::map<int, std::pair<int, std::string>>	_error_pages;
		size_t										_client_max_body_size;
		MimeTypes									_types;
		std::vector<Location>						_locations;
	public:
		Server(Http &http);
		~Server();
		Server(const Server &copy);
		Server	&operator=(const Server &other);

		const std::vector<int>								&listen() const;
		std::vector<int>									&listen();
		const std::vector<std::string>						&serverNames() const;
		std::vector<std::string>							&serverNames();
		const std::string									&root() const;
		std::string											&root();
		const std::vector<std::string>						&indexFiles() const;
		std::vector<std::string>							&indexFiles();
		const std::map<int, std::pair<int, std::string>>	&errorPages() const;
		std::map<int, std::pair<int, std::string>>			&errorPages();
		const size_t										&clientMaxBodySize() const;
		size_t												&clientMaxBodySize();
		const MimeTypes										&types() const;
		MimeTypes											&types();
		const std::vector<Location>							&locations() const;
		std::vector<Location>								&locations();
};