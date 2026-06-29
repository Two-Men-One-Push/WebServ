#pragma once

#include "model/MimeTypes/MimeTypes.hpp"
#include "model/Location/Location.hpp"
#include <vector>
#include <string>

class	Http;

class	Server
{
	private:
		std::vector<int>							_listen;
		std::vector<std::string>					_server_names;
		std::string									_root;
		std::vector<std::string>					_index_files;
		std::map<int, std::pair<int, std::string> >	_error_pages;
		size_t										_client_max_body_size;
		std::vector<std::string>					_allowed_methods;
		bool										_autoindex;
		std::pair<int, std::string>					_redirection;
		std::map<std::string, std::string>			_cgi;
		std::string									_upload_path;
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
		const std::map<int, std::pair<int, std::string> >	&errorPages() const;
		std::map<int, std::pair<int, std::string> >			&errorPages();
		const size_t										&clientMaxBodySize() const;
		size_t												&clientMaxBodySize();
		const std::vector<std::string>						&allowedMethods() const;
		std::vector<std::string>							&allowedMethods();
		const bool											&autoindex() const;
		bool												&autoindex();
		const std::pair<int, std::string>					&redirection() const;
		std::pair<int, std::string>							&redirection();
		const std::map<std::string, std::string>			&cgi() const;
		std::map<std::string, std::string>					&cgi();
		const std::string									&uploadPath() const;
		std::string											&uploadPath();
		const MimeTypes										&types() const;
		MimeTypes											&types();
		const std::vector<Location>							&locations() const;
		std::vector<Location>								&locations();
};