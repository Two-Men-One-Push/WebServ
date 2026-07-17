#pragma once

#include "model/Server/Server.hpp"
#include <vector>

class	Http: public Location
{
	private:
		std::vector<Server>	_servers;
	public:
		Http();
		~Http();
		Http(const Http &copy);
		Http	&operator=(const Http &other);

		const std::vector<Server>	&servers() const;
		std::vector<Server>			&servers();
};