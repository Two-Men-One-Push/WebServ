#include "Location.hpp"

Location::Location(): _locations()
{
}

Location::~Location()
{
}

void	Location::addLocation(const Location &location)
{
	_locations.push_back(location);
}

const std::vector<Location>	&Location::getLocations() const
{
	return (_locations);
}