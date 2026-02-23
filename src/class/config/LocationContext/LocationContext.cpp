#include "LocationContext.hpp"

LocationContext::~LocationContext()
{}

LocationContext::LocationContext():
_path(),
_methods(),
_root(),
_index(),
_autoindex(),
_redirection(),
_cgi_info()
{}

LocationContext::LocationContext(const LocationContext &copy):
_path(copy._path),
_methods(copy._methods),
_root(copy._root),
_index(copy._index),
_autoindex(copy._autoindex),
_redirection(copy._redirection),
_cgi_info(copy._cgi_info)
{}

LocationContext	&LocationContext::operator=(const LocationContext &other)
{
	if (this != &other)
	{
		this->_path = other._path;
		this->_methods = other._methods;
		this->_root = other._root;
		this->_index = other._index;
		this->_autoindex = other._autoindex;
		this->_redirection = other._redirection;
		this->_cgi_info = other._cgi_info;
	}
	return *this;
}