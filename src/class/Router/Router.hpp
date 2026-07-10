#pragma once

#include "model/Location/Location.hpp"
#include "URL/URL.hpp"

class	Router
{
	private:
	public:
		Router();
		~Router();

		static const Location	&resolveLocation(const URL &url, const std::vector<Location> &locations);
		static size_t	matchLength(const URL &url, const std::string &locationPath);
};