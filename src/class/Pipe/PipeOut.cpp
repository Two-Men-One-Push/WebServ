#include "Pipe.hpp"
#include <iostream>
#include <sys/epoll.h>

Pipe::Out::Out(int fd, Pipe::IPipeReader &target) : AFd(fd), _target(target) {}

Pipe::Out::~Out() {}

uint32_t Pipe::Out::getHandledEvents() const {
	return EPOLLIN;
}

void Pipe::Out::handleEvents(uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLIN) {
		this->_target.onPipeOut(*this);
	} else if (events & EPOLLERR) {
		std::cerr << "EPOLLERR unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLHUP) {
		std::cerr << "EPOLLHUP unused by PipeIn instance" << std::endl;
	}
}
