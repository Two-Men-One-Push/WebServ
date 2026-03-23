#include "Pipe.hpp"
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>

Pipe::PipeIn::PipeIn(int fd) : AFd(fd) {}

Pipe::PipeIn::~PipeIn() {}

uint32_t Pipe::PipeIn::getHandledEvents() const {
	return EPOLLIN;
}

void Pipe::PipeIn::handleEvents(uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLIN) {
		std::cerr << "EPOLLIN unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLERR) {
		std::cerr << "EPOLLERR unused by PipeIn instance" << std::endl;
	} else if (events & EPOLLHUP) {
		std::cerr << "EPOLLHUP unused by PipeIn instance" << std::endl;
	}
}
