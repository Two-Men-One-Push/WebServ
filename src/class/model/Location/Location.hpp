#pragma once

#include "MimeType.hpp"
#include <vector>

class	Location
{
	private:
		std::string							_path;
		std::string							_root;
		std::vector<std::string>			_index_files;
		std::map<int, std::string>			_error_pages;
		size_t								_client_max_body_size;
		std::vector<Location>				_locations;
		std::vector<std::string>			_allowed_methods;
		bool								_autoindex;
		std::pair<int, std::string>			_redirection;
		MimeType							_mimeType;
		std::map<std::string, std::string>	_cgi;
		std::string							_upload_path;
	public:
		Location();
		~Location();
		Location(const Location &copy);
		Location	&operator=(const Location &other);

		const std::vector<Location>	&locations() const;
		std::vector<Location>		&locations();
		const MimeType				&mimetype() const;
		MimeType					&mimetype();
};