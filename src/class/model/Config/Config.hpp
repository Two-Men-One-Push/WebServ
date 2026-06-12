#pragma once

#include "Http.hpp"
#include "MimeType.hpp"

class	Config
{
	private:
		bool		_error;
		Http		_http;
		bool		_httpSet;
		MimeType	_mimeType;
	public:
		Config();
		~Config();

		const Http	&getHttp() const;
		const MimeType	&getMimeType() const;
		bool	isHttpSet() const;
		void	setHttp(const Http &http);
		void	setMimeType(const MimeType &mimeType);
};