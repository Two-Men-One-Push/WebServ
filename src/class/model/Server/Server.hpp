#pragma once

#include "Location.hpp"
#include <vector>
#include <string>

class	Server
{
	private:
		std::vector<Location>		_locations;
		std::vector<std::string>	_listen;
		MimeType					_mimeType;
	public:
		Server();
		~Server();
		Server(const Server &copy);
		Server	&operator=(const Server &other);

		const std::vector<std::string>	&listen() const;
		std::vector<std::string>		&listen();
		const std::vector<Location>		&locations() const;
		std::vector<Location>			&locations();
		const MimeType					&mimetype() const;
		MimeType						&mimetype();
};