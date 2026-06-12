#include "Location.hpp"

Location::Location(): _locations(), _mimeType()
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

const MimeType	&Location::getMimeType() const
{
	return (this->_mimeType);
}

void	Location::setMimeType(const MimeType &mimeType)
{
	this->_mimeType = mimeType;
}