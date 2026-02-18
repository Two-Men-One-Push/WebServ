#include "./Epoll.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <sys/epoll.h>

Epoll::Epoll() : AFd(epoll_create(0)) {
}

static int Epoll::createEpoll() {
	const int epollFd = epoll_create(0);
	if (epollFd < 0) {
		throw std::runtime_error("Couldn't create epoll fd: " + strerror(errno));
	}
}
