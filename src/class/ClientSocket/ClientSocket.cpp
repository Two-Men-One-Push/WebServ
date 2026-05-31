#include "./ClientSocket.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpConnection.hpp"
#include "http/messages/HttpRequest.hpp"
#include <cerrno>
#include <stdint.h>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen)
	: ASocket(fd), _address(address), _addressLen(addressLen), _closed(false), _buffer(), _connections() {}

ClientSocket::~ClientSocket() {}

socklen_t ClientSocket::getAdressLen() const {
	return _addressLen;
}

ClientSocket *ClientSocket::createFromListener(int listenerFd) {
	struct sockaddr_storage clientAddr;
	socklen_t addrLen = sizeof(clientAddr);

	const int fd = accept(listenerFd, reinterpret_cast<struct sockaddr *>(&clientAddr), &addrLen);

	if (fd < 0)
		throw WebservErrors::SysError("accept", errno);

	return new ClientSocket(fd, clientAddr, addrLen);
}

const struct sockaddr_storage &ClientSocket::getAdress() const {
	return _address;
}

uint32_t ClientSocket::getHandledEvents() const {
	uint32_t result = EPOLLIN | EPOLLRDHUP;
	if (!this->_connections.empty() && this->_connections.front()->request().completed())
		result |= EPOLLOUT;
	return result;
}

void ClientSocket::handleEvents(u_int32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & (EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
		if (events & EPOLLIN) {
			std::cout << "EPOLLIN" << std::endl;
		}
		if (events & EPOLLOUT) {
			std::cout << "EPOLLOUT" << std::endl;
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
			this->onEpollIn(webServer);
		}
		if (events & EPOLLOUT) {
			this->onEpollOut(webServer);
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

void ClientSocket::onEpollIn(WebServer &webServer) {
	char buffer[BUFFER_SIZE];

	errno = 0;
	ssize_t readLen = read(_fd, buffer, BUFFER_SIZE);
	if (!readLen) {
		this->_closed = true;
	}
	if (readLen < 0) {
		std::cout << _closed << std::endl;
		throw WebservErrors::SysError("read", errno); // !:! warn instead and close connection
	}
	if (this->_closed) return;

	this->_buffer.write(buffer, readLen);
	if (this->_connections.empty()) {
		this->_connections.push(new HttpConnection());
	}
	while (this->_buffer.peek() != std::stringstream::traits_type::eof()) {
		if (this->_connections.back()->request().append(this->_buffer)) {
			webServer.updateFd(*this);
			this->_connections.push(new HttpConnection());
		}
	}
}

#define TEST_RESPONSE "HTTP/1.1 200 OK\r\n"         \
					  "Content-Type: text/json\r\n" \
					  "\r\n"                        \
					  "{\"hello\": \"world\"}"

void ClientSocket::onEpollOut(WebServer &webServer) {
	while (!this->_connections.empty() && this->_connections.front()->request().completed()) {
		HttpConnection *conn = this->_connections.front();
		delete conn;
		this->_connections.pop();
	}
	if (this->_connections.empty() || !this->_connections.front()->request().completed()) {
		webServer.updateFd(*this);
	}


}
