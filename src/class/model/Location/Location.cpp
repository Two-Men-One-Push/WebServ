#include "model/Location/Location.hpp"
#include "http/HttpStatus.hpp"

Location::Location():
_path(""),
_root(""),
_alias(""),
_index_files(),
_error_pages(),
_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE),
_allowed_methods(),
_autoindex(false),
_redirection(""),
_cgi(),
_editable(false),
_types()
{}

Location::Location(Location &parent, const std::string &path):
_path(path),
_root(parent._root),
_alias(parent._alias),
_index_files(),
_error_pages(parent._error_pages),
_max_body_size(parent._max_body_size),
_allowed_methods(),
_autoindex(parent.autoindex()),
_redirection(""),
_cgi(),
_editable(false),
_types(parent.types())
{}

Location::~Location()
{
}

Location::Location(const Location &copy):
_path(copy._path),
_root(copy._root),
_alias(copy._alias),
_index_files(copy._index_files),
_error_pages(copy._error_pages),
_max_body_size(copy._max_body_size),
_allowed_methods(copy._allowed_methods),
_autoindex(copy._autoindex),
_redirection(copy._redirection),
_cgi(copy._cgi),
_editable(copy._editable),
_types(copy._types)
{
}

Location	&Location::operator=(const Location &other)
{
	if (this != &other)
	{
		this->_path = other._path;
		this->_root = other._root;
		this->_alias = other._alias;
		this->_index_files = other._index_files;
		this->_error_pages = other._error_pages;
		this->_max_body_size = other._max_body_size;
		this->_allowed_methods = other._allowed_methods;
		this->_autoindex = other._autoindex;
		this->_redirection = other._redirection;
		this->_cgi = other._cgi;
		this->_editable = other._editable;
		this->_types = other._types;
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

const std::string	&Location::alias() const
{
	return (this->_alias);
}

std::string	&Location::alias()
{
	return (this->_alias);
}


const std::vector<std::string>	&Location::indexFiles() const
{
	return (this->_index_files);
}

std::vector<std::string>	&Location::indexFiles()
{
	return (this->_index_files);
}

const std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >	&Location::errorPages() const
{
	return (this->_error_pages);
}

std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >	&Location::errorPages()
{
	return (this->_error_pages);
}

const size_t	&Location::maxBodySize() const
{
	return (this->_max_body_size);
}

size_t	&Location::maxBodySize()
{
	return (this->_max_body_size);
}

const std::vector<HttpMethod>	&Location::allowedMethods() const
{
	return (this->_allowed_methods);
}

std::vector<HttpMethod>	&Location::allowedMethods()
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

const std::string	&Location::redirection() const
{
	return (this->_redirection);
}

std::string	&Location::redirection()
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

const bool	&Location::editable() const
{
	return (this->_editable);
}

bool	&Location::editable()
{
	return (this->_editable);
}

const MimeTypes	&Location::types() const
{
	return (this->_types);
}

MimeTypes	&Location::types()
{
	return (this->_types);
}
