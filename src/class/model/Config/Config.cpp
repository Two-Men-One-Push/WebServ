#include "Config.hpp"
#include "Http.hpp"
#include "MimeType.hpp"

Config::Config(): _http(), _httpSet(false), _mimeType()
{
}

Config::~Config()
{
}

const Http	&Config::getHttp() const
{
	return (this->_http);
}

const MimeType	&Config::getMimeType() const
{
	return (this->_mimeType);
}

bool	Config::isHttpSet() const
{
	return (this->_httpSet);
}

void	Config::setHttp(const Http &http)
{
	this->_http = http;
	this->_httpSet = true;
}

void	Config::setMimeType(const MimeType &mimeType)
{
	this->_mimeType = mimeType;
}