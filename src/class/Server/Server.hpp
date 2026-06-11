#pragma once

#include "Location.hpp"
#include <vector>
#include <string>

class	Server
{
	private:
		std::vector<Location>	_locations;
		int						_listen;
	public:
		Server();
		~Server();
	
		void	addLocation(const Location &location);
		const std::vector<Location>	&getLocations() const;
};