#pragma once

#include <map>
#include <string>

class	MimeType
{
	private:
		bool								_error;
		std::map<std::string, std::string>	_mimeTypes;
	public:
		MimeType();
		~MimeType();
		MimeType(const MimeType &copy);
		MimeType	&operator=(const MimeType &other);

		const std::map<std::string, std::string>	&getMimeTypes() const;
		void	addMimeType(const std::string &extension, const std::string &mimeType);
};