#pragma once

#include "http/types.hpp"
#include "model/MimeTypes/MimeTypes.hpp"
#include "http/HttpStatus.hpp"
#include <vector>

#define DEFAULT_CLIENT_MAX_BODY_SIZE 1000000

class	Http;

class	Location
{
	private:
		std::string																_path;
		std::string																_root;
		std::vector<std::string>												_index_files;
		std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >	_error_pages;
		size_t																	_client_max_body_size;
		std::vector<HttpMethod>													_allowed_methods;
		bool																	_autoindex;
		std::pair<HttpStatus::Code, std::string>								_redirection;
		std::map<std::string, std::string>										_cgi;
		bool																	_editable;
		MimeTypes																_types;
	public:
		Location(Location &parent, const std::string &path);
		Location();
		~Location();
		Location(const Location &copy);
		Location	&operator=(const Location &other);

		const std::string															&path() const;
		std::string																	&path();
		const std::string															&root() const;
		std::string																	&root();
		const std::vector<std::string>												&indexFiles() const;
		std::vector<std::string>													&indexFiles();
		const std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >	&errorPages() const;
		std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >		&errorPages();
		const size_t																&clientMaxBodySize() const;
		size_t																		&clientMaxBodySize();
		const std::vector<HttpMethod>												&allowedMethods() const;
		std::vector<HttpMethod>														&allowedMethods();
		const bool																	&autoindex() const;
		bool																		&autoindex();
		const std::pair<HttpStatus::Code, std::string>								&redirection() const;
		std::pair<HttpStatus::Code, std::string>									&redirection();
		const std::map<std::string, std::string>									&cgi() const;
		std::map<std::string, std::string>											&cgi();
		const bool																	&editable() const;
		bool																		&editable();
		const MimeTypes																&types() const;
		MimeTypes																	&types();
};