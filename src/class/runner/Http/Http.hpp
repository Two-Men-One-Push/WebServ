#ifndef HTTP_HPP
#define HTTP_HPP

#include "runner/Server/Server.hpp"
#include <vector>

class Http {
  private:
	
	std::vector<Server> _serverRunners;

  public:
	Http(/*HttpContext*/);
	~Http();
};

#endif
