#include "Router.hpp"
#include <vector>
#include "URL/URL.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "utils/parsing.hpp"
#include "model/Server/Server.hpp"
#include "model/Location/Location.hpp"
#include "Ressource/Ressource.hpp"
#include "http/HttpStatus.hpp"
#include <sys/stat.h>
#include "utils/parsing.hpp"

Router::Router()
{
}

Router::~Router()
{
}

size_t	matchLength(const URL &url, const std::string &locationPath)
{
	size_t match_length = 0;
	std::string	normalized_locationPath;
	pathNormalize(normalized_locationPath, locationPath);
	std::vector<std::string>	location_segment = splitPath(normalized_locationPath);
	std::vector<std::string>::const_iterator it_location = location_segment.begin();
	for (std::vector<std::string>::const_iterator it = url.normalizedSegments().begin(); it != url.normalizedSegments().end() && it_location != location_segment.end(); ++it)
	{
		if (it->compare(*it_location) == 0)
		{
			match_length++;
			it_location++;
		}
		else
			break;
	}
	if (it_location != location_segment.end())
		match_length = 0;
	return match_length;
}

std::string	getFileExtension(const std::string &path)
{
	std::string	last_segment;
	size_t last_segment_pos = path.find_last_of('/');
	if (last_segment_pos != std::string::npos)
		last_segment = path.substr(last_segment_pos + 1);
	size_t dot_pos = last_segment.find_last_of('.');
	if (dot_pos == std::string::npos || dot_pos == 0)
		return "";
	return last_segment.substr(dot_pos + 1);
}

Ressource	&getErrorPage(Ressource &ressource, const Location &location, HttpStatus::Code errorCode)
{
	ressource.type() = RESSOURCE_ERROR;
	ressource.responseCode() = errorCode;
	ressource.mimeType() = "text/html";
	std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> >::const_iterator it = location.errorPages().find(errorCode);
	if (it != location.errorPages().end())
	{
		ressource.path() = it->second.second;
		ressource.responseCode() = it->second.first;
	}
	return ressource;
}

Ressource	Router::resolveRessource(const HttpRequest &req, const Server &server)
{
	Ressource	ressource;
	size_t	longestMatchLength = 0;
	const Location	*bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it)
	{
		size_t match_length = matchLength(req.uri(), it->path());
		if (longestMatchLength < match_length)
		{
			longestMatchLength = match_length;
			bestMatch = &(*it);
		}
	}
	const Location	&location = *bestMatch;
	if (!location.allowedMethods().empty())
	{
		bool methodeAllowed = false;
		for (std::vector<std::string>::const_iterator it = location.allowedMethods().begin(); it != location.allowedMethods().end(); ++it)
		{
			if (req.methodStr() == *it)
			{
				methodeAllowed = true;
				break;
			}
		}
		if (!methodeAllowed)
		{
			return getErrorPage(ressource, location, HttpStatus::MethodNotAllowed);
		}
	}
	if (location.redirection().first != HttpStatus::NoStatus)
	{
		ressource.type() = RESSOURCE_REDIRECT;
		ressource.responseCode() = location.redirection().first;
		ressource.path() = location.redirection().second;
		return ressource;
	}
	std::string	cgiScriptPath = location.root();
	for (std::vector<std::string>::const_iterator it = req.uri().rawSegments().begin(); it != req.uri().rawSegments().end(); ++it)
	{
		std::string	decodedSegment;
		URL::decode(decodedSegment, *it);
		cgiScriptPath += "/" + decodedSegment;
		if (decodedSegment == ".." || decodedSegment == ".")
			continue;
		std::map<std::string, std::string>::const_iterator cgi_it = location.cgi().find(getFileExtension(cgiScriptPath));
		if (cgi_it != location.cgi().end())
		{
			struct stat cgiScript_stat;
			if (stat(cgiScriptPath.c_str(), &cgiScript_stat) == 0 && (cgiScript_stat.st_mode & S_IFREG))
			{
				ressource.type() = RESSOURCE_CGI;
				ressource.cgiInterpreter() = cgi_it->second;
				ressource.path() = cgiScriptPath;
				std::string	pathInfo;
				it++;
				for (std::vector<std::string>::const_iterator pathinfo_it = it; pathinfo_it != req.uri().rawSegments().end(); ++pathinfo_it)
				{
					pathInfo += "/" + *pathinfo_it;
				}
				ressource.pathInfo() = pathInfo;
				return ressource;
			}
		}
	}
	std::string path = location.root();
	for (std::vector<std::string>::const_iterator it = req.uri().normalizedSegments().begin(); it != req.uri().normalizedSegments().end(); ++it)
		path += "/" + *it;
	struct stat path_stat;
	if (stat(path.c_str(), &path_stat) == 0)
	{
		if (path_stat.st_mode & S_IFDIR)
		{
			if (!location.indexFiles().empty())
			{
				for (std::vector<std::string>::const_iterator it = location.indexFiles().begin(); it != location.indexFiles().end(); ++it)
				{
					std::string indexPath = location.root() + "/" + *it;
					struct stat index_stat;
					if (stat(indexPath.c_str(), &index_stat) == 0 && (index_stat.st_mode & S_IFREG))
					{
						ressource.type() = RESSOURCE_STATIC_FILE;
						ressource.responseCode() = HttpStatus::OK;
						ressource.path() = indexPath;
						std::map<std::string, std::string>::const_iterator type_it = location.types().types().find(getFileExtension(indexPath));
						if (type_it != location.types().types().end())
							ressource.mimeType() = type_it->second;
						else
							ressource.mimeType() = "application/octet-stream";
						return ressource;
					}
				}
			}
			if (location.autoindex())
			{
				ressource.type() = RESSOURCE_AUTO_INDEX;
				ressource.responseCode() = HttpStatus::OK;
				ressource.path() = path;
				return ressource;
			}
			else
			{
				return getErrorPage(ressource, location, HttpStatus::Forbidden);
			}
		}
		else if (path_stat.st_mode & S_IFREG)
		{
			ressource.type() = RESSOURCE_STATIC_FILE;
			ressource.responseCode() = HttpStatus::OK;
			ressource.path() = path;
			std::map<std::string, std::string>::const_iterator it = location.types().types().find(getFileExtension(path));
			if (it != location.types().types().end())
				ressource.mimeType() = it->second;
			else
				ressource.mimeType() = "application/octet-stream";
			return ressource;
		}
		else
		{
			return getErrorPage(ressource, location, HttpStatus::NotFound);
		}
	}
	else
	{
		return getErrorPage(ressource, location, HttpStatus::NotFound);
	}
}

Ressource	Router::resolveErrorRessource(const HttpRequest &req, HttpStatus::Code errorCode, const Server &server)
{
	Ressource	ressource;
	size_t	longestMatchLength = 0;
	const Location	*bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it)
	{
		size_t match_length = matchLength(req.uri(), it->path());
		if (longestMatchLength < match_length)
		{
			longestMatchLength = match_length;
			bestMatch = &(*it);
		}
	}
	const Location	&location = *bestMatch;
	return getErrorPage(ressource, location, errorCode);
}
