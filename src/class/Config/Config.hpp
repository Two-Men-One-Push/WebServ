#pragma once

#include "Server.hpp"
#include <vector>

class	Config
{
	private:
		std::vector<Server>	_servers;
	public:
		Config();
		~Config();
	
		void	addServer(const Server &server);
		const std::vector<Server>	&getServers() const;
};