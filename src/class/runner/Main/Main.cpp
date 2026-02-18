#include "./Main.hpp"
#include "AFd/AFd.hpp"
#include "runner/Main/Epoll/Epoll.hpp"
#include <sys/epoll.h>

Main::Main() : epoll(Epoll::createEpoll()) {
}
