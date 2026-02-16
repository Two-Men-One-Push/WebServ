#include "./Epoll.hpp"
#include <cstdlib>
#include <stdexcept>
#include <sys/epoll.h>

Epoll::Epoll() : AFd(epoll_create(0)) {

}

int Epoll::createEpoll() {
	const int epollFd = epoll_create(0);
	if (epollFd < 0) {
		// throw new std::system("")
	}
}
