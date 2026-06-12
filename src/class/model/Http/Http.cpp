#include "Http.hpp"

Http::Http(): _servers(), _mimetype()
{
}

Http::~Http()
{
}

Http::Http(const Http &copy): _servers(copy._servers), _mimetype(copy._mimetype)
{
}

Http	&Http::operator=(const Http &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
		this->_mimetype = other._mimetype;
	}
	return (*this);
}

const std::vector<Server>	&Http::servers() const
{
	return (this->_servers);
}

std::vector<Server>	&Http::servers()
{
	return (this->_servers);
}

const MimeType	&Http::mimetype() const
{
	return (this->_mimetype);
}

MimeType	&Http::mimetype()
{
	return (this->_mimetype);
}