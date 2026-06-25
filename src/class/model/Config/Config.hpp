#pragma once

#include "Http.hpp"

class	Config
{
	private:
	DiagnosticContext	_diag;
	Http				_http;
	public:
		Config();
		~Config();
		Config(const Config &copy);
		Config	&operator=(const Config &other);

		const DiagnosticContext	&diag() const;
		DiagnosticContext	&diag();
		const Http	&http() const;
		Http		&http();
};