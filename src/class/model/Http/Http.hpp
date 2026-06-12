#pragma once

#include "Server.hpp"
#include "MimeType.hpp"
#include <vector>

class	Http
{
	private:
		std::vector<Server>	_servers;
		MimeType			_mimetype;
	public:
		Http();
		~Http();
		Http(const Http &copy);
		Http	&operator=(const Http &other);

		const std::vector<Server>	&servers() const;
		std::vector<Server>			&servers();
		const MimeType				&mimetype() const;
		MimeType					&mimetype();
};