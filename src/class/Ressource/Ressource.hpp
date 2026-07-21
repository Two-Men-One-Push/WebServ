#pragma once

#include <string>
#include "http/HttpStatus.hpp"

typedef enum
{
	RESSOURCE_NONE,
	RESSOURCE_STATIC_FILE,
	RESSOURCE_CGI,
	RESSOURCE_REDIRECT,
	RESSOURCE_ERROR,
	RESSOURCE_AUTO_INDEX,
}	RessourceType;

class	Ressource
{
	private:
		RessourceType		_type;
		std::string			_path;
		std::string			_mimeType;
		HttpStatus::Code	_responseCode;
		std::string			_cgiInterpreter;
		std::string			_scriptName;
		std::string			_pathInfo;
		std::string			_fragmentString;
	public:
		Ressource();
		~Ressource();

		const std::string		typeStr() const;

		const RessourceType		&type() const;
		RessourceType			&type();
		const std::string		&path() const;
		std::string				&path();
		const std::string		&mimeType() const;
		std::string				&mimeType();
		const HttpStatus::Code	&responseCode() const;
		HttpStatus::Code		&responseCode();
		const std::string		&cgiInterpreter() const;
		std::string				&cgiInterpreter();
		const std::string		&scriptName() const;
		std::string				&scriptName();
		const std::string		&pathInfo() const;
		std::string				&pathInfo();
};
