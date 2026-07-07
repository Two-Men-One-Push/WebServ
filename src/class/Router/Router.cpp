#include "Router.hpp"
#include <iostream>
#include <vector>
#include "utils/parsing.hpp"

Router::Router()
{
}

Router::~Router()
{
}

const Location	&Router::resolveLocation(const URL &url, const std::vector<Location> &locations)
{
	size_t longestMatchLength = 0;
	const Location *bestMatch = NULL;
	for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
	{
		size_t match_lenght = matchLength(url, it->path());
		if (longestMatchLength < match_lenght || bestMatch == NULL)
		{
			longestMatchLength = match_lenght;
			bestMatch = &(*it);
		}
	}
	return *bestMatch;
}

size_t	Router::matchLength(const URL &url, const std::string &locationPath)
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
	std::cout << "match_length: " << match_length << " for location: " << locationPath << std::endl;
	return match_length;
}