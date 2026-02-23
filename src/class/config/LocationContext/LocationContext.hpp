#ifndef LOCATIONCONTEXT_HPP
#define LOCATIONCONTEXT_HPP

#include <string>
#include <vector>
#include <map>

class	LocationContext
{
	private:
		std::string							_path;
		std::vector<std::string>			_methods;
		std::string							_root;
		std::string							_index;
		bool								_autoindex;
		std::string							_redirection;
		std::map<std::string, std::string>	_cgi_info;
	public:
		~LocationContext();
		LocationContext();
		LocationContext(const LocationContext &copy);
		LocationContext	&operator=(const LocationContext &other);
};

#endif