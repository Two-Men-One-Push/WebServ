#include "Server.hpp"
#include "Location.hpp"

template <typename Type>
Location::Location(Type &parent, std::string path):
_path(path),
_root(path),
_index_files(),
_error_pages(),
_client_max_body_size(parent.clientMaxBodySize()),
_allowed_methods(),
_autoindex(false),
_redirection(),
_cgi(),
_upload_path(),
_types(parent.types()),
_locations()
{
}

Location::~Location()
{
}

Location::Location(const Location &copy):
_path(copy._path),
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

Location	&Location::operator=(const Location &other)
{
	if (this != &other)
	{
		this->_path = other._path;
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

const std::string	&Location::path() const
{
	return (this->_path);
}

std::string	&Location::path()
{
	return (this->_path);
}

const std::string	&Location::root() const
{
	return (this->_root);
}

std::string	&Location::root()
{
	return (this->_root);
}

const std::vector<std::string>	&Location::indexFiles() const
{
	return (this->_index_files);
}

std::vector<std::string>	&Location::indexFiles()
{
	return (this->_index_files);
}

const std::map<int, std::pair<int, std::string>>	&Location::errorPages() const
{
	return (this->_error_pages);
}

std::map<int, std::pair<int, std::string>>	&Location::errorPages()
{
	return (this->_error_pages);
}

const size_t	&Location::clientMaxBodySize() const
{
	return (this->_client_max_body_size);
}

size_t	&Location::clientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const std::vector<std::string>	&Location::allowedMethods() const
{
	return (this->_allowed_methods);
}

std::vector<std::string>	&Location::allowedMethods()
{
	return (this->_allowed_methods);
}

const bool	&Location::autoindex() const
{
	return (this->_autoindex);
}

bool	&Location::autoindex()
{
	return (this->_autoindex);
}

const std::pair<int, std::string>	&Location::redirection() const
{
	return (this->_redirection);
}

std::pair<int, std::string>	&Location::redirection()
{
	return (this->_redirection);
}

const std::map<std::string, std::string>	&Location::cgi() const
{
	return (this->_cgi);
}

std::map<std::string, std::string>	&Location::cgi()
{
	return (this->_cgi);
}

const std::string	&Location::uploadPath() const
{
	return (this->_upload_path);
}

std::string	&Location::uploadPath()
{
	return (this->_upload_path);
}

const MimeType	&Location::mimetype() const
{
	return (this->_mimeType);
}

MimeType	&Location::mimetype()
{
	return (this->_mimeType);
}

const std::vector<Location>	&Location::locations() const
{
	return (this->_locations);
}

std::vector<Location>	&Location::locations()
{
	return (this->_locations);
}
