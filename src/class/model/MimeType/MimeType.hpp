#pragma once

#include <map>
#include <string>

class	MimeType
{
	private:
		std::map<std::string, std::string>	_mimeTypes;
	public:
		MimeType();
		~MimeType();
		MimeType(const MimeType &copy);
		MimeType	&operator=(const MimeType &other);

		const std::map<std::string, std::string>	&mimetypes() const;
		std::map<std::string, std::string>			&mimetypes();
};