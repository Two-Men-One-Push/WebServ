#include "Http.hpp"

Http::Http(): _servers(), _mimeType()
{
}

Http::~Http()
{
}

Http::Http(const Http &copy): _servers(copy._servers), _mimeType(copy._mimeType)
{
}

Http	&Http::operator=(const Http &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
		this->_mimeType = other._mimeType;
	}
	return (*this);
}

void	Http::addServer(const Server &server)
{
	this->_servers.push_back(server);
}

const std::vector<Server>	&Http::getServers() const
{
	return (this->_servers);
}

const MimeType	&Http::getMimeType() const
{
	return (this->_mimeType);
}

void	Http::setMimeType(const MimeType &mimeType)
{
	this->_mimeType = mimeType;
}