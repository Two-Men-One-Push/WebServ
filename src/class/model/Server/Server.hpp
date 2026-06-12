#pragma once

#include "Location.hpp"
#include <vector>
#include <string>

class	Server
{
	private:
		bool						_error;
		std::vector<Location>		_locations;
		std::vector<std::string>	_listen;
		MimeType					_mimeType;
	public:
		Server();
		~Server();
	
		const std::vector<std::string>	&getListen() const;
		const std::vector<Location>	&getLocations() const;
		const MimeType	&getMimeType() const;
		void	addListen(const std::string &listen);
		void	addLocation(const Location &location);
		void	setMimeType(const MimeType &mimeType);
};