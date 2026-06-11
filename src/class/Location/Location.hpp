#pragma once

#include <vector>

class	Location
{
	private:
		std::vector<Location>	_locations;
	public:
		Location();
		~Location();

		void	addLocation(const Location &location);
		const std::vector<Location>	&getLocations() const;
};