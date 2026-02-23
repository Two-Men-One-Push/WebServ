#include "./EpollInstance.hpp"
#include "AFd/AFd.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <sys/epoll.h>

EpollInstance::EpollInstance(int epollFd) : AFd(epollFd) {}

EpollInstance::~EpollInstance() {}

void EpollInstance::registerFd(AFd &fd) {
	epoll_event epollEvent = {
		.events = EPOLLET,
		.data = {
			.ptr = (&fd),
		},
	};

	epoll_ctl(_fd, EPOLL_CTL_ADD, fd.getFd(), &epollEvent);
}

EpollInstance EpollInstance::create() {
	const int epollFd = epoll_create(1);
	if (epollFd < 0) {
		throw webserv_errors::SysError("epoll", errno);
	}

	return EpollInstance(epollFd);
}

AFd &EpollInstance::wait() const {
	epoll_event epollEventBuffer[1];

	epoll_wait(_fd, epollEventBuffer, 1, -1);
	return *static_cast<AFd*>(epollEventBuffer->data.ptr);
}
