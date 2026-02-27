#ifndef MAINCONTEXT_CPP
#define MAINCONTEXT_CPP

#include <vector>
#include "ServerContext.hpp"

class	MainContext
{
	private:
		std::vector<ServerContext>	_servers;
	public:
		~MainContext();
		MainContext();
		MainContext(const MainContext &copy);
		MainContext	&operator=(const MainContext &other);
		MainContext(std::istream &file);
};

#endif