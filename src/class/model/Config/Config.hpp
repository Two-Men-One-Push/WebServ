#pragma once

#include "model/Http/Http.hpp"

class	Config
{
	private:
		Http				_http;
	public:
		Config();
		~Config();
		Config(const Config &copy);
		Config	&operator=(const Config &other);

		const Http	&http() const;
		Http		&http();
};