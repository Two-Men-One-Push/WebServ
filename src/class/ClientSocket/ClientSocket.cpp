#include "./ClientSocket.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/HttpRequest.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen)
	: ASocket(fd), _address(address), _addressLen(addressLen), _closed(false), _iBuffer(), _transactions() {}

ClientSocket::~ClientSocket() {
	while (!this->_transactions.empty()) {
		delete this->_transactions.front();
		this->_transactions.pop();
	}
}

socklen_t ClientSocket::addressLen() const {
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

const struct sockaddr_storage &ClientSocket::address() const {
	return _address;
}

uint32_t ClientSocket::getHandledEvents() const {
	uint32_t result = EPOLLIN | EPOLLRDHUP;
	if (this->canHandleEpollOut())
		result |= EPOLLOUT;
	return result;
}

bool ClientSocket::canHandleEpollOut() const {
	return !this->_transactions.empty() && this->_transactions.front()->request().completed();
}

void ClientSocket::handleEvents(u_int32_t events, WebServer &webServer) {
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
			webServer.requestDelete(this);
		}
		if (events & EPOLLHUP) {
			webServer.requestDelete(this);
		}
		if (events & EPOLLERR) {
			webServer.requestDelete(this);
		}
	} else {
		std::cout << "Unhandled event : " << events << std::endl;
	}
}

#define BUFFER_SIZE 4096

void ClientSocket::onEpollIn(WebServer &webServer) {
	char buffer[BUFFER_SIZE];
	std::stringstream &iBuffer = this->_iBuffer;

	errno = 0;
	ssize_t readLen = read(_fd, buffer, BUFFER_SIZE);

	if (!readLen) this->_closed = true;
	if (readLen < 0) {
		std::cout << _closed << std::endl;
		throw WebservErrors::SysError("read", errno); // !:! warn instead and close connection
	}
	if (this->_closed) return;

	iBuffer.clear();
	iBuffer.write(buffer, readLen);

	if (this->_transactions.empty()) {
		this->_transactions.push(new HttpTransaction());
	}
	while (iBuffer.peek() != std::stringstream::traits_type::eof()) {
		if (this->_transactions.back()->request().append(iBuffer)) {
			this->_transactions.push(new HttpTransaction());
		}
	}

	if (this->canHandleEpollOut()) {
		webServer.updateFd(*this);
	}
}

#define TEST_RESPONSE "HTTP/1.1 200 OK\r\n"         \
					  "Content-Type: text/json\r\n" \
					  "\r\n"                        \
					  "{\"hello\": \"world\"}"

void ClientSocket::onEpollOut(WebServer &webServer) {
	while (!this->_transactions.empty() && this->_transactions.front()->request().completed()) {
		HttpTransaction *transaction = this->_transactions.front();
		delete transaction;
		this->_transactions.pop();
	}
	if (!this->canHandleEpollOut()) {
		webServer.updateFd(*this);
	}
}
