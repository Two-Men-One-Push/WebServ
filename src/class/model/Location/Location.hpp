#pragma once

#include "MimeType.hpp"
#include <vector>

class	Location
{
	private:
		std::vector<Location>	_locations;
		MimeType				_mimeType;
	public:
		Location();
		~Location();
		Location(const Location &copy);
		Location	&operator=(const Location &other);

		const std::vector<Location>	&locations() const;
		std::vector<Location>		&locations();
		const MimeType				&mimetype() const;
		MimeType					&mimetype();
};