#include "Ressource/Ressource.hpp"
#include "http/HttpStatus.hpp"

Ressource::Ressource():
_type(RESSOURCE_NONE),
_path(""),
_mimeType(""),
_responseCode(HttpStatus::NoStatus),
_cgiInterpreter(""),
_pathInfo("")
{
}

Ressource::~Ressource()
{
}

const std::string	Ressource::typeStr() const
{
	switch (this->_type)
	{
		case RESSOURCE_NONE:
			return ("RESSOURCE_NONE");
		case RESSOURCE_STATIC_FILE:
			return ("RESSOURCE_STATIC_FILE");
		case RESSOURCE_CGI:
			return ("RESSOURCE_CGI");
		case RESSOURCE_REDIRECT:
			return ("RESSOURCE_REDIRECT");
		case RESSOURCE_ERROR:
			return ("RESSOURCE_ERROR");
		case RESSOURCE_AUTO_INDEX:
			return ("RESSOURCE_AUTO_INDEX");
		default:
			return ("RESSOURCE_UNKNOWN");
	}
	return ("RESSOURCE_UNKNOWN");
}

const RessourceType	&Ressource::type() const
{
	return (this->_type);
}

RessourceType	&Ressource::type()
{
	return (this->_type);
}

const std::string	&Ressource::path() const
{
	return (this->_path);
}

std::string	&Ressource::path()
{
	return (this->_path);
}

const std::string	&Ressource::mimeType() const
{
	return (this->_mimeType);
}

std::string	&Ressource::mimeType()
{
	return (this->_mimeType);
}

const HttpStatus::Code	&Ressource::responseCode() const
{
	return (this->_responseCode);
}

HttpStatus::Code	&Ressource::responseCode()
{
	return (this->_responseCode);
}

const std::string	&Ressource::cgiInterpreter() const
{
	return (this->_cgiInterpreter);
}

std::string	&Ressource::cgiInterpreter()
{
	return (this->_cgiInterpreter);
}

const std::string	&Ressource::pathInfo() const
{
	return (this->_pathInfo);
}

std::string	&Ressource::pathInfo()
{
	return (this->_pathInfo);
}
