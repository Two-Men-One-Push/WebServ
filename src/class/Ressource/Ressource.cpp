#include "Ressource/Ressource.hpp"

Ressource::Ressource():
_type(RESSOURCE_STATIC_FILE),
_path(""),
_errorPath(""),
_autoIndexPath(""),
_mimeType(""),
_redirectPath(""),
_responseCode(-1),
_cgiPath(""),
_pathInfo(""),
_queryString("")
{
}

Ressource::~Ressource()
{
}

const std::string	Ressource::typeStr() const
{
	switch (this->_type)
	{
		case RESSOURCE_STATIC_FILE:
			return ("RESSOURCE_STATIC_FILE");
		case RESSOURCE_CGI:
			return ("RESSOURCE_CGI");
		case RESSOURCE_REDIRECT:
			return ("RESSOURCE_REDIRECT");
		case RESSOURCE_ERROR_:
			return ("RESSOURCE_ERROR_");
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

const std::string	&Ressource::errorPath() const
{
	return (this->_errorPath);
}

std::string	&Ressource::errorPath()
{
	return (this->_errorPath);
}

const std::string	&Ressource::autoIndexPath() const
{
	return (this->_autoIndexPath);
}

std::string	&Ressource::autoIndexPath()
{
	return (this->_autoIndexPath);
}

const std::string	&Ressource::mimeType() const
{
	return (this->_mimeType);
}

std::string	&Ressource::mimeType()
{
	return (this->_mimeType);
}

const std::string	&Ressource::redirectPath() const
{
	return (this->_redirectPath);
}

std::string	&Ressource::redirectPath()
{
	return (this->_redirectPath);
}

const int	&Ressource::responseCode() const
{
	return (this->_responseCode);
}

int	&Ressource::responseCode()
{
	return (this->_responseCode);
}

const std::string	&Ressource::cgiPath() const
{
	return (this->_cgiPath);
}

std::string	&Ressource::cgiPath()
{
	return (this->_cgiPath);
}

const std::string	&Ressource::pathInfo() const
{
	return (this->_pathInfo);
}

std::string	&Ressource::pathInfo()
{
	return (this->_pathInfo);
}

const std::string	&Ressource::queryString() const
{
	return (this->_queryString);
}

std::string	&Ressource::queryString()
{
	return (this->_queryString);
}