#include "Config.hpp"

Config::Config(): _servers()
{
}

Config::~Config()
{
}

void	Config::addServer(const Server &server)
{
	_servers.push_back(server);
}

const std::vector<Server>	&Config::getServers() const
{
	return (_servers);
}