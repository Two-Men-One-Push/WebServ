#include "Server.hpp"

Server::Server(): _locations()
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