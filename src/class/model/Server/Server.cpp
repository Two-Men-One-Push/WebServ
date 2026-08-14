#include "model/Server/Server.hpp"
#include "model/Http/Http.hpp"

#

Server::Server(Http &http):
Location(http, "/"),
_listen(),
_server_names(),
_locations(),
_timeout(DEFAULT_TIMEOUT)
{}

Server::~Server()
{}

Server::Server(const Server &copy):
Location(copy),
_listen(copy._listen),
_server_names(copy._server_names),
_locations(copy._locations),
_timeout(copy._timeout)
{}

Server	&Server::operator=(const Server &other)
{
	if (this != &other)
	{
		this->_listen = other._listen;
		this->_server_names = other._server_names;
		this->_locations = other._locations;
		this->_timeout = other._timeout;
		Location::operator=(other);
	}
	return (*this);
}

const std::vector<std::pair<std::string, int> >	&Server::listen() const
{
	return (this->_listen);
}

std::vector<std::pair<std::string, int> >	&Server::listen()
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

const std::vector<Location>	&Server::locations() const
{
	return (this->_locations);
}

std::vector<Location>	&Server::locations()
{
	return (this->_locations);
}

const std::time_t	&Server::timeout() const
{
	return (this->_timeout);
}

std::time_t	&Server::timeout()
{
	return (this->_timeout);
}