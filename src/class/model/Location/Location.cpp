#include "Location.hpp"

Location::Location(): _locations(), _mimeType()
{
}

Location::~Location()
{
}

Location::Location(const Location &copy): _locations(copy._locations), _mimeType(copy._mimeType)
{
}

Location	&Location::operator=(const Location &other)
{
	if (this != &other)
	{
		this->_locations = other._locations;
		this->_mimeType = other._mimeType;
	}
	return (*this);
}

const std::vector<Location>	&Location::locations() const
{
	return (this->_locations);
}

std::vector<Location>	&Location::locations()
{
	return (this->_locations);
}

const MimeType	&Location::mimetype() const
{
	return (this->_mimeType);
}

MimeType	&Location::mimetype()
{
	return (this->_mimeType);
}