#include "EpollInstance/EpollWatchable.hpp"
#include "Pipe.hpp"
#include <sys/epoll.h>

Pipe::In::In(int fd, Pipe::IPipeWriter &target) : AEpollWatchable(fd), _target(target) {}

Pipe::In::~In() {}

uint32_t Pipe::In::getHandledEvents() const {
	return EPOLLOUT;
}

void Pipe::In::handleEvents(uint32_t events, WebServer &webServer) {
	this->_target.inPipeEvent(*this, events, webServer);
}
