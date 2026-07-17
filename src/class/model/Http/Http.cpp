#include "Http.hpp"
#include "model/Location/Location.hpp"

Http::Http(): Location(), _servers()
{
}

Http::~Http()
{
}

Http::Http(const Http &copy): Location(copy), _servers(copy._servers)
{
}

Http	&Http::operator=(const Http &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
		Location::operator=(other);
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