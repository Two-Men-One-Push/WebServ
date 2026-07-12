#pragma once

#include <string>

typedef enum
{
	RESSOURCE_STATIC_FILE,
	RESSOURCE_CGI,
	RESSOURCE_REDIRECT,
	RESSOURCE_ERROR_,
	RESSOURCE_AUTO_INDEX,
}	RessourceType;

class	Ressource
{
	private:
		RessourceType	_type;
		std::string		_path;
		std::string		_errorPath;
		std::string		_autoIndexPath;
		std::string		_mimeType;
		std::string		_redirectPath;
		int				_responseCode;
		std::string		_cgiPath;
		std::string		_pathInfo;
		std::string		_queryString;
	public:
		Ressource();
		~Ressource();

		const std::string	typeStr() const;

		const RessourceType	&type() const;
		RessourceType		&type();
		const std::string	&path() const;
		std::string			&path();
		const std::string	&errorPath() const;
		std::string			&errorPath();
		const std::string	&autoIndexPath() const;
		std::string			&autoIndexPath();
		const std::string	&mimeType() const;
		std::string			&mimeType();
		const std::string	&redirectPath() const;
		std::string			&redirectPath();
		const int			&responseCode() const;
		int					&responseCode();
		const std::string	&cgiPath() const;
		std::string			&cgiPath();
		const std::string	&pathInfo() const;
		std::string			&pathInfo();
		const std::string	&queryString() const;
		std::string			&queryString();
};