#include "Server.hpp"

Server::Server(): _locations(), _listen(), _mimeType()
{
}

Server::~Server()
{
}

Server::Server(const Server &copy): _locations(copy._locations), _listen(copy._listen), _mimeType(copy._mimeType)
{
}

Server	&Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_locations = other._locations;
		this->_listen = other._listen;
		this->_mimeType = other._mimeType;
	}
	return (*this);
}

const std::vector<std::string>	&Server::listen() const
{
	return (this->_listen);
}

std::vector<std::string>	&Server::listen()
{
	return (this->_listen);
}

const std::vector<Location>	&Server::locations() const
{
	return (this->_locations);
}

std::vector<Location>	&Server::locations()
{
	return (this->_locations);
}

const MimeType	&Server::mimetype() const
{
	return (this->_mimeType);
}

MimeType	&Server::mimetype()
{
	return (this->_mimeType);
}