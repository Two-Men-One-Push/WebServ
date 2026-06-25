#include "Config.hpp"
#include "Http.hpp"
#include "MimeType.hpp"

Config::Config(): _diag(), _http()
{
}

Config::~Config()
{
}

Config::Config(const Config &copy): _diag(copy._diag), _http(copy._http)
{
}

Config	&Config::operator=(const Config &other)
{
	if (this != &other)
	{
		this->_diag = other._diag;
		this->_http = other._http;
	}
	return (*this);
}

const DiagnosticContext	&Config::diag() const
{
	return (this->_diag);
}

DiagnosticContext	&Config::diag()
{
	return (this->_diag);
}

const Http	&Config::http() const
{
	return (this->_http);
}

Http	&Config::http()
{
	return (this->_http);
}