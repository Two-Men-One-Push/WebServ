#include "./EpollInstance.hpp"
#include "Fd/Fd.hpp"
#include "EpollInstance/EpollWatchable.hpp"
#include "errors/WebservErrors.hpp"
#include <cassert>
#include <cerrno>
#include <sys/epoll.h>
#include <vector>

int EpollInstance::createEpollFd() {
	const int epollFd = epoll_create(1);

	if (epollFd < 0) throw WebservErrors::SysError("epoll", errno);
	return epollFd;
}

EpollInstance::EpollInstance() : Fd(EpollInstance::createEpollFd()) {}

EpollInstance::~EpollInstance() {}


void EpollInstance::add(AEpollWatchable &watchable) const {
	if (watchable.epoll() != NULL) throw WebservErrors::Runtime("EpollWatchable is already registered to epoll");

	epoll_event epollEvent = {
		.events = watchable.getHandledEvents(),
		.data = {.ptr = (&watchable)},
	};

	errno = 0;
	if (epoll_ctl(_fd, EPOLL_CTL_ADD, watchable.fd(), &epollEvent)) throw WebservErrors::SysError("epoll_ctl", errno);
	watchable.epoll(this);
}

void EpollInstance::mod(AEpollWatchable &watchable) const {
	if (watchable.epoll() == NULL) throw WebservErrors::Runtime("EpollWatchable has not been registered to epoll yet");

	epoll_event epollEvent = {
		.events = watchable.getHandledEvents(),
		.data = {.ptr = (&watchable)},
	};

	errno = 0;
	if (epoll_ctl(_fd, EPOLL_CTL_MOD, watchable.fd(), &epollEvent) == -1) throw WebservErrors::SysError("epoll_ctl", errno);
	watchable.epoll(this);
}

void EpollInstance::del(int fd) const {
	errno = 0;
	if (epoll_ctl(_fd, EPOLL_CTL_DEL, fd, NULL) == -1) throw WebservErrors::SysError("epoll_ctl", errno);
}

void EpollInstance::del(AEpollWatchable &watchable) const {
	if (watchable.epoll() == NULL) throw WebservErrors::Runtime("EpollWatchable is not registered to epoll");

	errno = 0;
	if (epoll_ctl(_fd, EPOLL_CTL_DEL, watchable.fd(), NULL) == -1) throw WebservErrors::SysError("epoll_ctl", errno);
	watchable.epoll(NULL);
}

#define MAX_EVENTS 10
#define EPOLL_TIMEOUT 500 // ms

void EpollInstance::wait(std::vector<EpollEvent> &result) const {
	epoll_event epollEventBuffer[MAX_EVENTS];

	int eventCount = epoll_wait(_fd, epollEventBuffer, MAX_EVENTS, EPOLL_TIMEOUT);

	if (eventCount < 0) throw WebservErrors::SysError("epoll_wait", errno);

	result.reserve(eventCount);

	for (int i = 0; i < eventCount; i++) {
		result.push_back((EpollEvent){
			epollEventBuffer[i].events,
			static_cast<AEpollWatchable *>(epollEventBuffer[i].data.ptr)});
	}
}
