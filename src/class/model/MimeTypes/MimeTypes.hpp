#pragma once

#include <map>
#include <string>

class	MimeTypes
{
	private:
		std::map<std::string, std::string>	_types;
	public:
		MimeTypes();
		~MimeTypes();
		MimeTypes(const MimeTypes &copy);
		MimeTypes	&operator=(const MimeTypes &other);

		void	clear();

		const std::map<std::string, std::string>	&types() const;
		std::map<std::string, std::string>			&types();
};