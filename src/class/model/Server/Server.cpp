#include "model/Server/Server.hpp"
#include "model/Http/Http.hpp"

Server::Server(Http &http):
_listen(),
_server_names(),
_root(),
_index_files(),
_error_pages(http.errorPages()),
_client_max_body_size(http.clientMaxBodySize()),
_allowed_methods(),
_autoindex(false),
_redirection(0, ""),
_cgi(),
_upload_path(),
_types(http.types()),
_locations()
{
}

Server::~Server()
{
}

Server::Server(const Server &copy):
_listen(copy._listen),
_server_names(copy._server_names),
_root(copy._root),
_index_files(copy._index_files),
_error_pages(copy._error_pages),
_client_max_body_size(copy._client_max_body_size),
_allowed_methods(copy._allowed_methods),
_autoindex(copy._autoindex),
_redirection(copy._redirection),
_cgi(copy._cgi),
_upload_path(copy._upload_path),
_types(copy._types),
_locations(copy._locations)
{
}

Server	&Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_listen = other._listen;
		this->_server_names = other._server_names;
		this->_root = other._root;
		this->_index_files = other._index_files;
		this->_error_pages = other._error_pages;
		this->_client_max_body_size = other._client_max_body_size;
		this->_allowed_methods = other._allowed_methods;
		this->_autoindex = other._autoindex;
		this->_redirection = other._redirection;
		this->_cgi = other._cgi;
		this->_upload_path = other._upload_path;
		this->_types = other._types;
		this->_locations = other._locations;
	}
	return (*this);
}

const std::string										Server::path() const
{
	return "/";
}

const std::vector<int>	&Server::listen() const
{
	return (this->_listen);
}

std::vector<int>	&Server::listen()
{
	return (this->_listen);
}

const std::vector<std::string>	&Server::serverNames() const
{
	return (this->_server_names);
}

std::vector<std::string>	&Server::serverNames()
{
	return (this->_server_names);
}

const std::string	&Server::root() const
{
	return (this->_root);
}

std::string	&Server::root()
{
	return (this->_root);
}

const std::vector<std::string>	&Server::indexFiles() const
{
	return (this->_index_files);
}

std::vector<std::string>	&Server::indexFiles()
{
	return (this->_index_files);
}

const std::map<int, std::pair<int, std::string> >	&Server::errorPages() const
{
	return (this->_error_pages);
}

std::map<int, std::pair<int, std::string> >	&Server::errorPages()
{
	return (this->_error_pages);
}

const size_t	&Server::clientMaxBodySize() const
{
	return (this->_client_max_body_size);
}

size_t	&Server::clientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const std::vector<std::string>	&Server::allowedMethods() const
{
	return (this->_allowed_methods);
}

std::vector<std::string>	&Server::allowedMethods()
{
	return (this->_allowed_methods);
}

const bool	&Server::autoindex() const
{
	return (this->_autoindex);
}

bool	&Server::autoindex()
{
	return (this->_autoindex);
}

const std::pair<int, std::string>	&Server::redirection() const
{
	return (this->_redirection);
}

std::pair<int, std::string>	&Server::redirection()
{
	return (this->_redirection);
}

const std::map<std::string, std::string>	&Server::cgi() const
{
	return (this->_cgi);
}

std::map<std::string, std::string>	&Server::cgi()
{
	return (this->_cgi);
}

const std::string	&Server::uploadPath() const
{
	return (this->_upload_path);
}

std::string	&Server::uploadPath()
{
	return (this->_upload_path);
}

const MimeTypes	&Server::types() const
{
	return (this->_types);
}

MimeTypes	&Server::types()
{
	return (this->_types);
}

const std::vector<Location>	&Server::locations() const
{
	return (this->_locations);
}

std::vector<Location>	&Server::locations()
{
	return (this->_locations);
}