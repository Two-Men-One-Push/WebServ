#pragma once

#include "MimeType.hpp"
#include <vector>

class	Location
{
	private:
		bool					_error;
		std::vector<Location>	_locations;
		MimeType				_mimeType;
	public:
		Location();
		~Location();

		void	addLocation(const Location &location);
		const std::vector<Location>	&getLocations() const;
		const MimeType	&getMimeType() const;
		void	setMimeType(const MimeType &mimeType);
};