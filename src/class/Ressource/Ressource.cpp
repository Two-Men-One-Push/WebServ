#include "Ressource/Ressource.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/types.hpp"
#include "model/Server/Server.hpp"
#include "utils/parsing.hpp"
#include <fcntl.h>
#include <sys/stat.h>

size_t matchLength(const URL &url, const std::string &locationPath) {
	size_t match_length = 0;
	std::string normalized_locationPath;
	pathNormalize(normalized_locationPath, locationPath);
	std::vector<std::string> location_segment = splitPath(normalized_locationPath);
	std::vector<std::string>::const_iterator it_location = location_segment.begin();
	for (std::vector<std::string>::const_iterator it = url.normalizedSegments().begin(); it != url.normalizedSegments().end() && it_location != location_segment.end(); ++it) {
		if (it->compare(*it_location) == 0) {
			match_length++;
			it_location++;
		} else
			break;
	}
	if (it_location != location_segment.end())
		match_length = 0;
	return match_length;
}

std::string getFileExtension(const std::string &path) {
	std::string last_segment;
	size_t last_segment_pos = path.find_last_of('/');
	if (last_segment_pos != std::string::npos)
		last_segment = path.substr(last_segment_pos + 1);
	size_t dot_pos = last_segment.find_last_of('.');
	if (dot_pos == std::string::npos || dot_pos == 0)
		return "";
	return last_segment.substr(dot_pos + 1);
}

void Ressource::setErrorPage(const Location &location, HttpStatus::Code errorCode) {
	this->_type = RESSOURCE_ERROR;
	this->_responseCode = errorCode;
	this->_mimeType = "text/html";
	std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >::const_iterator it = location.errorPages().find(errorCode);
	if (it != location.errorPages().end()) {
		this->_root = location.root();
		this->_path = it->second.second;
		this->_responseCode = it->second.first;
	}
}

Ressource::Ressource(const HttpRequest &req, const Server &server)
	: _type(RESSOURCE_NONE),
	_root(""),
	_path(""),
	_mimeType(""),
	_responseCode(HttpStatus::NoStatus),
	_cgiInterpreter(""),
	_scriptName(""),
	_pathInfo(""),
	_allowedMethod(),
	_location()	{
	size_t longestMatchLength = 0;
	const Location *bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it) {
		size_t match_length = matchLength(req.uri(), it->path());
		if (longestMatchLength < match_length) {
			longestMatchLength = match_length;
			bestMatch = &(*it);
		}
	}
	const Location &location = *bestMatch;
	this->_location = location;
	this->_allowedMethod = location.allowedMethods();
	if (!location.allowedMethods().empty()) {
		bool methodeAllowed = false;
		for (std::vector<HttpMethod>::const_iterator it = location.allowedMethods().begin(); it != location.allowedMethods().end(); ++it) {
			if (req.method() == *it) {
				methodeAllowed = true;
				break;
			}
		}
		if (!methodeAllowed) {
			this->setErrorPage(location, HttpStatus::MethodNotAllowed);
			return;
		}
	}
	if (location.redirection().first != HttpStatus::NoStatus) {
		this->_type = RESSOURCE_REDIRECT;
		this->_responseCode = location.redirection().first;
		this->_path = location.redirection().second;
		return;
	}
	std::string cgiScriptPath;
	for (std::vector<std::string>::const_iterator it = req.uri().rawSegments().begin(); it != req.uri().rawSegments().end(); ++it) {
		std::string decodedSegment;
		URL::decode(decodedSegment, *it);
		cgiScriptPath += "/" + decodedSegment;
		if (decodedSegment == ".." || decodedSegment == ".")
			continue;
		std::map<std::string, std::string>::const_iterator cgi_it = location.cgi().find(getFileExtension(cgiScriptPath));
		if (cgi_it != location.cgi().end()) {
			struct stat cgiScript_stat;
			if (::stat(cgiScriptPath.c_str(), &cgiScript_stat) == 0 && (cgiScript_stat.st_mode & S_IFREG)) {
				this->_type = RESSOURCE_CGI;
				this->_cgiInterpreter = cgi_it->second;
				this->_root = location.root();
				this->_path = cgiScriptPath;
				std::string pathInfo;
				std::string scriptName;
				it++;
				for (std::vector<std::string>::const_iterator script_it = req.uri().rawSegments().begin(); script_it != it; ++script_it) {
					scriptName += "/" + *script_it;
				}
				std::string decodedScriptName;
				URL::decode(decodedScriptName, scriptName);
				this->scriptName() = scriptName;
				for (std::vector<std::string>::const_iterator pathinfo_it = it; pathinfo_it != req.uri().rawSegments().end(); ++pathinfo_it) {
					pathInfo += "/" + *pathinfo_it;
				}
				std::string decodedPathInfo;
				URL::decode(decodedPathInfo, pathInfo);
				this->pathInfo() = decodedPathInfo;
				return;
			}
		}
	}
	std::string path = location.root();
	std::string	request_path = "";
	for (std::vector<std::string>::const_iterator it = req.uri().normalizedSegments().begin(); it != req.uri().normalizedSegments().end(); ++it)
		request_path += "/" + *it;
	if (path != "/")
		path += request_path;
	else
		path = request_path;
	if (req.method() == POST)
	{
		if (!location.editable())
			this->setErrorPage(location, HttpStatus::MethodNotAllowed);
		this->_type = RESSOURCE_UPLOAD;
		this->_root = location.root();
		this->_path = request_path;
		this->_responseCode = HttpStatus::Created;
		this->_mimeType = "text/html";
	}
	else if (location.editable() && req.method() == DELETE)
	{
		if (!location.editable())
			this->setErrorPage(location, HttpStatus::MethodNotAllowed);
		this->_type = RESSOURCE_DELETE;
		this->_root = location.root();
		this->_path = request_path;
		this->_responseCode = HttpStatus::NoContent;
		this->_mimeType = "text/html";
	}
	else if (req.method() == GET || req.method() == HEAD)
	{
		struct stat path_stat;
		if (stat(path.c_str(), &path_stat) == 0) {
			if (path_stat.st_mode & S_IFDIR) {
				if (!location.indexFiles().empty()) {
					for (std::vector<std::string>::const_iterator it = location.indexFiles().begin(); it != location.indexFiles().end(); ++it) {
						std::string indexPath = "/" + *it;
						struct stat index_stat;
						if (stat(indexPath.c_str(), &index_stat) == 0 && (index_stat.st_mode & S_IFREG)) {
							this->type() = RESSOURCE_STATIC_FILE;
							this->responseCode() = HttpStatus::OK;
							this->_root = location.root();
							this->_path = indexPath;
							std::map<std::string, std::string>::const_iterator type_it = location.types().types().find(getFileExtension(indexPath));
							if (type_it != location.types().types().end())
								this->mimeType() = type_it->second;
							else
								this->mimeType() = "application/octet-stream";
							return;
						}
					}
				}
				if (location.autoindex()) {
					this->type() = RESSOURCE_AUTO_INDEX;
					this->responseCode() = HttpStatus::OK;
					this->_root = location.root();
					this->_path = request_path;
					return;
				} else {
					this->setErrorPage(location, HttpStatus::Forbidden);
					return;
				}
			} else if (path_stat.st_mode & S_IFREG) {
				this->type() = RESSOURCE_STATIC_FILE;
				this->responseCode() = HttpStatus::OK;
				this->_root = location.root();
				this->_path = request_path;
				std::map<std::string, std::string>::const_iterator it = location.types().types().find(getFileExtension(path));
				if (it != location.types().types().end())
					this->mimeType() = it->second;
				else
					this->mimeType() = "application/octet-stream";
				return;
			} else {
				this->setErrorPage(location, HttpStatus::NotFound);
				return;
			}
		} else {
			this->setErrorPage(location, HttpStatus::NotFound);
			return;
		}
	}
	else
	{
		this->setErrorPage(location, HttpStatus::NotImplemented);
	}
}

Ressource::Ressource(const HttpRequest &req, HttpStatus::Code errorCode, const Server &server)
	: _type(RESSOURCE_NONE),
	  _root(""),
	  _path(""),
	  _mimeType(""),
	  _responseCode(HttpStatus::NoStatus),
	  _cgiInterpreter(""),
	  _scriptName(""),
	  _pathInfo(""),
	  _fragmentString(""),
	  _allowedMethod(),
	  _location()	{
	size_t longestMatchLength = 0;
	const Location *bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it) {
		size_t match_length = matchLength(req.uri(), it->path());
		if (longestMatchLength < match_length) {
			longestMatchLength = match_length;
			bestMatch = &(*it);
		}
	}
	const Location &location = *bestMatch;
	this->_location = location;
	this->_allowedMethod = location.allowedMethods();
	this->setErrorPage(location, errorCode);
}

Ressource::~Ressource() {
}

const std::string Ressource::typeStr() const {
	switch (this->_type) {
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
	case RESSOURCE_UPLOAD:
		return ("RESSOURCE_UPLOAD");
	case RESSOURCE_DELETE:
		return ("RESSOURCE_DELETE");
	default:
		return ("RESSOURCE_UNKNOWN");
	}
	return ("RESSOURCE_UNKNOWN");
}

const RessourceType &Ressource::type() const {
	return (this->_type);
}

RessourceType &Ressource::type() {
	return (this->_type);
}

const std::string &Ressource::root() const {
	return (this->_root);
}

std::string &Ressource::root() {
	return (this->_root);
}

const std::string &Ressource::path() const {
	return (this->_path);
}

std::string &Ressource::path() {
	return (this->_path);
}

const std::string &Ressource::mimeType() const {
	return (this->_mimeType);
}

std::string &Ressource::mimeType() {
	return (this->_mimeType);
}

const HttpStatus::Code &Ressource::responseCode() const {
	return (this->_responseCode);
}

HttpStatus::Code &Ressource::responseCode() {
	return (this->_responseCode);
}

const std::string &Ressource::cgiInterpreter() const {
	return (this->_cgiInterpreter);
}

std::string &Ressource::cgiInterpreter() {
	return (this->_cgiInterpreter);
}

const std::string &Ressource::scriptName() const {
	return (this->_scriptName);
}

std::string &Ressource::scriptName() {
	return (this->_scriptName);
}

const std::string &Ressource::pathInfo() const {
	return (this->_pathInfo);
}

std::string &Ressource::pathInfo() {
	return (this->_pathInfo);
}

const std::vector<HttpMethod> &Ressource::allowedMethods() const {
	return (this->_allowedMethod);
}

std::vector<HttpMethod> &Ressource::allowedMethods() {
	return (this->_allowedMethod);
}

const Location &Ressource::location() const {
	return (this->_location);
}

Location &Ressource::location() {
	return (this->_location);
}