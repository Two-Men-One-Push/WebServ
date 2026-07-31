#pragma once

#include "http/HttpStatus.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "model/Server/Server.hpp"
#include <string>
#include <vector>

typedef enum {
	RESSOURCE_NONE,
	RESSOURCE_STATIC_FILE,
	RESSOURCE_CGI,
	RESSOURCE_REDIRECT,
	RESSOURCE_ERROR,
	RESSOURCE_AUTO_INDEX,
	RESSOURCE_UPLOAD,
	RESSOURCE_DELETE,
}	RessourceType;

class Ressource {
  private:
	RessourceType _type;
	std::string	_root;
	std::string _location;
	std::string _mimeType;
	HttpStatus::Code _responseCode;
	std::string _cgiInterpreter;
	std::string _scriptName;
	std::string _pathInfo;
	std::string _fragmentString;
	std::vector<HttpMethod>	_allowed_method;

	void setErrorPage(const Location &location, HttpStatus::Code errorCode);

  public:
	Ressource(const HttpRequest &req, const Server &server);
	Ressource(const HttpRequest &req, HttpStatus::Code errorCode, const Server &server);
	~Ressource();

	const std::string typeStr() const;

	const RessourceType &type() const;
	RessourceType &type();
	const std::string &root() const;
	std::string &root();
	const std::string &location() const;
	std::string &location();
	const std::string &mimeType() const;
	std::string &mimeType();
	const HttpStatus::Code &responseCode() const;
	HttpStatus::Code &responseCode();
	const std::string &cgiInterpreter() const;
	std::string &cgiInterpreter();
	const std::string &scriptName() const;
	std::string &scriptName();
	const std::string &pathInfo() const;
	std::string &pathInfo();
	const std::vector<HttpMethod> &allowedMethods() const;
	std::vector<HttpMethod> &allowedMethods();
};
