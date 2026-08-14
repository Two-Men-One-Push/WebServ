#pragma once

#include "model/Location/Location.hpp"
#include <ctime>
#include <vector>
#include <string>

#define DEFAULT_TIMEOUT 8

class	Http;

class	Server: public Location
{
	private:
		std::vector<std::pair<std::string, int> >	_listen;
		std::vector<std::string>					_server_names;
		std::vector<Location>						_locations;
		std::time_t									_timeout;
	public:
		Server(Http &http);
		~Server();
		Server(const Server &copy);
		Server	&operator=(const Server &other);

		const std::vector<std::pair<std::string, int> >		&listen() const;
		std::vector<std::pair<std::string, int> >			&listen();
		const std::vector<std::string>						&serverNames() const;
		std::vector<std::string>							&serverNames();
		const std::vector<Location>							&locations() const;
		std::vector<Location>								&locations();
		const std::time_t									&timeout() const;
		std::time_t											&timeout();
};
