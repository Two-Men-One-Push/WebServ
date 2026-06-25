#include "Server.hpp"
#include "Http.hpp"

Server::Server(Http &http):
_listen(),
_server_names(),
_root(),
_index_files(),
_error_pages(http.errorPages()),
_client_max_body_size(http.clientMaxBodySize()),
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
		this->_types = other._types;
		this->_locations = other._locations;
	}
	return (*this);
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