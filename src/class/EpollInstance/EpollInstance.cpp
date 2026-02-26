#include "./EpollInstance.hpp"
#include "AFd/AFd.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <sys/epoll.h>
#include <vector>

EpollInstance::EpollInstance(int epollFd) : AFd(epollFd) {}

EpollInstance::~EpollInstance() {}

EpollInstance EpollInstance::create() {
	const int epollFd = epoll_create(1);
	if (epollFd < 0) {
		throw webserv_errors::SysError("epoll", errno);
	}

	return EpollInstance(epollFd);
}

void EpollInstance::registerFd(AFd &fd) const {
	epoll_event epollEvent = {
		.events =  fd.getHandledEvents(),
		.data = {
			.ptr = (&fd),
		},
	};

	epoll_ctl(_fd, EPOLL_CTL_ADD, fd.getFd(), &epollEvent);
}

void EpollInstance::updateFd(AFd &fd) const {
	epoll_event epollEvent = {
		.events =  fd.getHandledEvents(),
		.data = {
			.ptr = (&fd),
		},
	};

	epoll_ctl(_fd, EPOLL_CTL_MOD, fd.getFd(), &epollEvent);
}

#define MAX_EVENTS 10

void EpollInstance::wait(std::vector<EpollEvent> &result) const {
	epoll_event epollEventBuffer[MAX_EVENTS];

	int eventCount = epoll_wait(_fd, epollEventBuffer, MAX_EVENTS, -1);

	if (eventCount < 0) throw webserv_errors::SysError("epoll_wait", errno);

	result.reserve(eventCount);

	for (int i = 0; i < eventCount; i++) {
		result.push_back((EpollEvent){
			epollEventBuffer[i].events,
			static_cast<AFd *>(epollEventBuffer[i].data.ptr)
		});
	}
}
