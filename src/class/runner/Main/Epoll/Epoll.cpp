#include "./Epoll.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <sys/epoll.h>

Epoll::Epoll(int epollFd) : AFd(epollFd) {}

Epoll::~Epoll() {}

Epoll Epoll::create() {
	const int epollFd = epoll_create(0);
	if (epollFd < 0) {
		throw webserv_errors::SysError("epoll", errno);
	}

	return Epoll(epollFd);
}
