#ifndef SERVERCONTEXT_CPP
#define SERVERCONTEXT_CPP

#include <string>
#include <vector>
#include <map>
#include "LocationContext.hpp"

class	ServerContext
{
	private:
		int								_port;
		std::string						_host;
		std::vector<std::string>		_server_names;
		std::map<int, std::string>		_error_pages;
		size_t							_client_max_body_size;
		std::vector<LocationContext>	_locations;
	public:
		~ServerContext();
		ServerContext();
		ServerContext(const ServerContext &copy);
		ServerContext	&operator=(const ServerContext &other);
};

#endif