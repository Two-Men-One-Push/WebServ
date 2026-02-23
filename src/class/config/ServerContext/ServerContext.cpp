#include "ServerContext.hpp"

ServerContext::~ServerContext()
{}

ServerContext::ServerContext():
_port(),
_host(),
_server_names(),
_error_pages(),
_client_max_body_size(),
_locations()
{}

ServerContext::ServerContext(const ServerContext &copy):
_port(copy._port),
_host(copy._host),
_server_names(copy._server_names),
_error_pages(copy._error_pages),
_client_max_body_size(copy._client_max_body_size),
_locations(copy._locations)
{}

ServerContext &ServerContext::operator=(const ServerContext &other)
{
	if (this != &other)
	{
		this->_port = other._port;
		this->_host = other._host;
		this->_server_names = other._server_names;
		this->_error_pages = other._error_pages;
		this->_client_max_body_size = other._client_max_body_size;
		this->_locations = other._locations;
	}
	return *this;
}