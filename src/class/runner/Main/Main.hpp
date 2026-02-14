#ifndef MAIN_HPP
#define MAIN_HPP

#include "config/MainContext/MainContext.hpp"
#include "runner/Http/Http.hpp"
#include <vector>

class Main {
  private:
	MainContext &_mainConfig;
	std::vector<Http> _httpRunners;

  public:
	Main(Config &config);
	~Main();
};

#endif
