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

Router::Router()
{
}

Router::~Router()
{
}

size_t	matchLength(const URL &url, const std::string &locationPath)
{
	size_t match_length = 0;
	std::vector<std::string>	location_segment = splitPath(locationPath);
	std::vector<std::string>::const_iterator it_location = location_segment.begin();
	for (std::vector<std::string>::const_iterator it = url.segments().begin(); it != url.segments().end() && it_location != location_segment.end(); ++it)
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
	return last_segment.substr(dot_pos);
}

Ressource	Router::resolveRessource(const HttpRequest &req, const Server &server)
{
	Ressource ressource;
	size_t longestMatchLength = 0;
	URL	url(req.uri());
	if (url.format() == URL_ERROR)
	{
		ressource.type() = RESSOURCE_ERROR;
		ressource.responseCode() = HttpStatus::BadRequest;
		ressource.mimeType() = "text/html";
		std::map<int, std::pair<int, std::string> >::const_iterator it = server.errorPages().find(HttpStatus::BadRequest);
		if (it != server.errorPages().end())
		{
			ressource.path() = it->second.second;
			ressource.responseCode() = it->second.first;
		}
		return ressource;
	}
	const Location *bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it)
	{
		size_t match_lenght = matchLength(url, it->path());
		if (longestMatchLength < match_lenght)
		{
			longestMatchLength = match_lenght;
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
			ressource.type() = RESSOURCE_ERROR;
			ressource.responseCode() = HttpStatus::MethodNotAllowed;
			ressource.mimeType() = "text/html";
			std::map<int, std::pair<int, std::string> >::const_iterator it = server.errorPages().find(HttpStatus::MethodNotAllowed);
			if (it != server.errorPages().end())
			{
				ressource.path() = it->second.second;
				ressource.responseCode() = it->second.first;
			}
			return ressource;
		}
	}
	if (location.redirection().first != -1)
	{
		ressource.type() = RESSOURCE_REDIRECT;
		ressource.responseCode() = location.redirection().first;
		ressource.path() = location.redirection().second;
		return ressource;
	}
	std::string path = location.root() + url.path();
	std::vector<std::string> location_segments = splitPath(location.path());
	std::vector<std::string> url_segments = url.segments();
	for (std::vector<std::string>::const_iterator it = location_segments.begin(); it != location_segments.end(); ++it)
	{
		if (url_segments.size() > 0 && *it == *url_segments.begin())
			url_segments.erase(url_segments.begin());
		else
			break;
	}
	std::string	cgiScriptPath = location.root() + location.path();
	for (std::vector<std::string>::const_iterator it = url_segments.begin(); it != url_segments.end(); ++it)
	{
		cgiScriptPath += "/" + *it;
		std::map<std::string, std::string>::const_iterator cgi_it = location.cgi().find(getFileExtension(cgiScriptPath));
		if (cgi_it != location.cgi().end())
		{
			struct stat cgiScript_stat;
			if (stat(cgiScriptPath.c_str(), &cgiScript_stat) == 0 && (cgiScript_stat.st_mode & S_IFREG))
			{
				ressource.type() = RESSOURCE_CGI;
				ressource.cgiInterpreter() = cgi_it->second;
				ressource.path() = cgiScriptPath;
				ressource.pathInfo() = path.substr(cgiScriptPath.length());
				ressource.queryString() = url.queryString();
				ressource.fragmentString() = url.fragmentString();
				return ressource;
			}
		}
	}
	struct stat path_stat;
	if (stat(path.c_str(), &path_stat) == 0)
	{
		if (path_stat.st_mode & S_IFDIR)
		{
			if (!location.indexFiles().empty())
			{
				for (std::vector<std::string>::const_iterator it = location.indexFiles().begin(); it != location.indexFiles().end(); ++it)
				{
					std::string indexPath = location.root() + location.path() + "/" + *it;
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
				ressource.type() = RESSOURCE_ERROR;
				ressource.responseCode() = HttpStatus::Forbidden;
				ressource.mimeType() = "text/html";
				std::map<int, std::pair<int, std::string> >::const_iterator it = server.errorPages().find(HttpStatus::Forbidden);
				if (it != server.errorPages().end())
				{
					ressource.path() = it->second.second;
					ressource.responseCode() = it->second.first;
				}
				return ressource;
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
			ressource.type() = RESSOURCE_ERROR;
			ressource.responseCode() = HttpStatus::NotFound;
			ressource.mimeType() = "text/html";
			std::map<int, std::pair<int, std::string> >::const_iterator it = server.errorPages().find(HttpStatus::NotFound);
			if (it != server.errorPages().end())
			{
				ressource.path() = it->second.second;
				ressource.responseCode() = it->second.first;
			}
			return ressource;
		}
	}
	else
	{
		ressource.type() = RESSOURCE_ERROR;
		ressource.responseCode() = HttpStatus::NotFound;
		ressource.mimeType() = "text/html";
		std::map<int, std::pair<int, std::string> >::const_iterator it = server.errorPages().find(HttpStatus::NotFound);
		if (it != server.errorPages().end())
		{
			ressource.path() = it->second.second;
			ressource.responseCode() = it->second.first;
		}
		return ressource;
	}
}