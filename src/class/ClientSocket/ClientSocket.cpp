#include "./ClientSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen)
	: ASocket(fd), _address(address), _addressLen(addressLen), _closed(false) {}

ClientSocket::~ClientSocket() {}

socklen_t ClientSocket::getAdressLen() const {
	return _addressLen;
}

ClientSocket *ClientSocket::createFromListener(int listenerFd) {
	struct sockaddr_storage clientAddr;
	socklen_t addrLen = sizeof(clientAddr);

	const int fd = accept(listenerFd, reinterpret_cast<struct sockaddr *>(&clientAddr), &addrLen);

	if (fd < 0)
		throw webserv_errors::SysError("accept", errno);

	return new ClientSocket(fd, clientAddr, addrLen);
}

const struct sockaddr_storage &ClientSocket::getAdress() const {
	return _address;
}

u_int32_t ClientSocket::getHandledEvents() const {
	return EPOLLIN | EPOLLOUT | EPOLLRDHUP;
}

void ClientSocket::handleEvents(u_int32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
		if (events & EPOLLIN) {
			std::cout << "EPOLLIN" << std::endl;
		}
		if (events & EPOLLOUT) {
			// std::cout << "EPOLLOUT" << std::endl;
		}
		if (events & EPOLLRDHUP) {
			std::cout << "EPOLLRDHUP" << std::endl;
		}
		if (events & EPOLLHUP) {
			std::cout << "EPOLLHUP" << std::endl;
		}
		if (events & EPOLLERR) {
			std::cout << "EPOLLERR" << std::endl;
		}
		if (events & EPOLLIN) {
			this->onEpollIn();
		}
		if (events & EPOLLOUT) {
			this->onEpollOut();
		}
		if (events & EPOLLRDHUP) {
			_closed = true;
		}
		if (events & EPOLLHUP) {
			std::cout << "Unhandled event : EPOLLHUP" << std::endl;
		}
		if (events & EPOLLERR) {
			std::cout << "Unhandled event : EPOLLERR" << std::endl;
		}
	} else {
		std::cout << "Unhandled event : " << events << std::endl;
	}
}

#define BUFFER_SIZE 4096

void ClientSocket::onEpollIn() {
	char buffer[BUFFER_SIZE];

	errno = 0;
	ssize_t readLen = read(_fd, buffer, BUFFER_SIZE);
	if (!readLen)
		_closed = true;
	else if (readLen > 0) {
		std::cout.write(buffer, readLen);
		readCount++;
	} else {
		std::cout << _closed << std::endl;
		throw webserv_errors::SysError("read", errno);
	}
}

#define TEST_RESPONSE "HTTP/1.1 200 OK\r\n"         \
					  "Content-Type: text/json\r\n" \
					  "\r\n"                        \
					  "{\"hello\": \"world\"}"

void ClientSocket::onEpollOut() {
	if (readCount > 0 && !responseSent) {
		// responseSent = true;
		write(_fd, TEST_RESPONSE, strlen(TEST_RESPONSE));
		close(_fd);
	}
}
