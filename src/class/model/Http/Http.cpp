#include "Http.hpp"

Http::Http(): _client_max_body_size(DEFAULT_CLIENT_MAX_BODY_SIZE), _error_pages(), _types(), _servers()
{
}

Http::~Http()
{
}

Http::Http(const Http &copy): _client_max_body_size(copy._client_max_body_size), _error_pages(copy._error_pages), _types(copy._types), _servers(copy._servers)
{
}

Http	&Http::operator=(const Http &other)
{
	if (this != &other)
	{
		this->_client_max_body_size = other._client_max_body_size;
		this->_error_pages = other._error_pages;
		this->_types = other._types;
		this->_servers = other._servers;
	}
	return (*this);
}

const size_t	&Http::clientMaxBodySize() const
{
	return (this->_client_max_body_size);
}

size_t	&Http::clientMaxBodySize()
{
	return (this->_client_max_body_size);
}

const std::map<int, std::pair<int, std::string> >	&Http::errorPages() const
{
	return (this->_error_pages);
}

std::map<int, std::pair<int, std::string> >	&Http::errorPages()
{
	return (this->_error_pages);
}

const std::vector<Server>	&Http::servers() const
{
	return (this->_servers);
}

std::vector<Server>	&Http::servers()
{
	return (this->_servers);
}

const MimeTypes	&Http::types() const
{
	return (this->_types);
}

MimeTypes	&Http::types()
{
	return (this->_types);
}