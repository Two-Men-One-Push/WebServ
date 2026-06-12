#include "Server.hpp"

Server::Server(): _locations(), _mimeType()
{
}

Server::~Server()
{
}

void	Server::addLocation(const Location &location)
{
	_locations.push_back(location);
}

const std::vector<Location>	&Server::getLocations() const
{
	return (_locations);
}

const MimeType	&Server::getMimeType() const
{
	return (this->_mimeType);
}

void	Server::setMimeType(const MimeType &mimeType)
{
	this->_mimeType = mimeType;
}

void	Server::addListen(const std::string &listen)
{
	this->_listen.push_back(listen);
}

const std::vector<std::string>	&Server::getListen() const
{
	return (this->_listen);
}