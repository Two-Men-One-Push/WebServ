#include "Pipe.hpp"
#include <iostream>
#include <sys/epoll.h>

Pipe::PipeOut::PipeOut(int fd) : AFd(fd) {}

Pipe::PipeOut::~PipeOut() {}

uint32_t Pipe::PipeOut::getHandledEvents() const {
	return EPOLLIN;
}

void Pipe::PipeOut::handleEvents(uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		std::cerr << "EPOLLOUT unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLERR) {
		std::cerr << "EPOLLERR unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLHUP) {
		std::cerr << "EPOLLHUP unused by PipeIn instance" << std::endl;
	}
}
