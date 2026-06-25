#include "Config.hpp"
#include "Http.hpp"
#include "MimeTypes.hpp"
#include "DiagnosticContext.hpp"

Config::Config(): _http()
{
}

Config::~Config()
{
}

Config::Config(const Config &copy): _http(copy._http)
{
}

Config	&Config::operator=(const Config &other)
{
	if (this != &other)
	{
		this->_http = other._http;
	}
	return (*this);
}

const Http	&Config::http() const
{
	return (this->_http);
}

Http	&Config::http()
{
	return (this->_http);
}