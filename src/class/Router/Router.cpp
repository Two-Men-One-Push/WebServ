#include "Router.hpp"
#include <vector>
#include "utils/parsing.hpp"
#include "model/Server/Server.hpp"
#include "model/Location/Location.hpp"
#include "Ressource/Ressource.hpp"

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
	for (std::vector<std::string>::const_iterator it = url.path().begin(); it != url.path().end() && it_location != location_segment.end(); ++it)
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

Ressource	Router::resolveRessource(const URL &url, const Server &server)
{
	Ressource ressource;
	size_t longestMatchLength = 0;
	const Location *bestMatch = &server;
	for (std::vector<Location>::const_iterator it = server.locations().begin(); it != server.locations().end(); ++it)
	{
		size_t match_lenght = matchLength(url, it->path());
		if (longestMatchLength < match_lenght || bestMatch == NULL)
		{
			longestMatchLength = match_lenght;
			bestMatch = &(*it);
		}
	}
	const Location	&location = *bestMatch;
	(void)location;
	return ressource;
}