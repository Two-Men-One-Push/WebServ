#include "./Main.hpp"
#include "config/MainContext/MainContext.hpp"
#include "runner/Main/Epoll/Epoll.hpp"
#include <sys/epoll.h>

Main::Main(Config &config) : epoll(Epoll::create()), _mainConfig(config) {}
