#pragma once

#include "Server.hpp"
#include "MimeType.hpp"
#include <vector>

class	Http
{
	private:
		bool				_error;
		std::vector<Server>	_servers;
		MimeType			_mimeType;
	public:
		Http();
		~Http();
		Http(const Http &copy);
		Http	&operator=(const Http &other);

		void	addServer(const Server &server);
		const std::vector<Server>	&getServers() const;
		const MimeType	&getMimeType() const;
		void	setMimeType(const MimeType &mimeType);
};