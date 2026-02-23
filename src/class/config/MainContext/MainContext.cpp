#include "MainContext.hpp"
#include <string>

MainContext::~MainContext()
{}

MainContext::MainContext(): _servers()
{}

MainContext::MainContext(const MainContext &copy): _servers(copy._servers)
{}

MainContext	&MainContext::operator=(const MainContext &other)
{
	if (this != &other)
	{
		this->_servers = other._servers;
	}
	return *this;
}

MainContext::MainContext(std::string file)
{
	(void)file;
}