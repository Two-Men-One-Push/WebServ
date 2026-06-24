#include "EpollInstance/EpollWatchable.hpp"
#include "Pipe.hpp"
#include <sys/epoll.h>

Pipe::Out::Out(int fd, Pipe::IPipeReader &target) : AEpollWatchable(fd), _target(target) {}

Pipe::Out::~Out() {}

uint32_t Pipe::Out::getHandledEvents() const {
	return EPOLLIN;
}

void Pipe::Out::handleEvents(uint32_t events, WebServer &webServer) {
	this->_target.outPipeEvent(*this, events, webServer);
}
