#include "Pipe.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

Pipe::In::In(int fd, Pipe::IPipeWriter &target) : AFd(fd), _target(target) {}

Pipe::In::~In() {}

uint32_t Pipe::In::getHandledEvents() const {
	return EPOLLOUT;
}

void Pipe::In::handleEvents(uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		this->_target.onPipeIn(*this);
	} else if (events & EPOLLERR) {
		std::cerr << "EPOLLERR unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLHUP) {
		std::cerr << "EPOLLHUP unused by PipeIn instance" << std::endl;
	}
}
