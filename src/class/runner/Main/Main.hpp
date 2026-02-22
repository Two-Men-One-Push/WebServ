#ifndef MAIN_HPP
#define MAIN_HPP

#include "AFd/AFd.hpp"
#include "config/MainContext/MainContext.hpp"
#include "runner/Http/Http.hpp"
#include "runner/Main/Epoll/Epoll.hpp"
#include <vector>

class Main {
  private:
	Epoll epoll;
	MainContext &_mainConfig;
	std::vector<Http> _httpRunners;

  public:
	Main(Config &config);
	~Main();
};

#endif
