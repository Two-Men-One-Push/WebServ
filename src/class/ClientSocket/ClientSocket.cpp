#include "./ClientSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpTransaction.hpp"
#include "utils/formatting.hpp"
#include <cerrno>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <queue>
#include <sstream>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen)
	: ASocket(fd), _address(address), _addressLen(addressLen), _closed(false), _outBuffer(), _transactions() {
	FormattedAddress formattedAddress;
	formatAddress(address, formattedAddress);
	std::cout << "New connection to " << formattedAddress.address << ':' << formattedAddress.port << " created" << std::endl;
}

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
	uint32_t result = EPOLLIN;
	if (this->canHandleEpollOut())
		result |= EPOLLOUT;
	return result;
}

bool ClientSocket::canHandleEpollOut() const {
	return !this->_transactions.empty() && this->_transactions.front()->response().inCompleted();
}

void ClientSocket::handleEvents(u_int32_t events, WebServer &webServer) {
	if (events & (EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR)) {
		if (events & EPOLLIN) {
			std::cout << "EPOLLIN" << std::endl;
		}
		if (events & EPOLLOUT) {
			std::cout << "EPOLLOUT" << std::endl;
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
		if (events & EPOLLHUP) {
			webServer.requestDelete(this);
		}
		if (events & EPOLLERR) {
			webServer.requestDelete(this);
		}
	} else {
		std::cerr << "Unhandled event : " << events << std::endl;
	}
}

#define BUFFER_SIZE 4096

void ClientSocket::onEpollIn(WebServer &server) {
	char buffer[BUFFER_SIZE];
	std::stringstream inBuffer;

	errno = 0;
	ssize_t readLen = this->read(buffer, BUFFER_SIZE);

	bool closed = false;

	if (!readLen) closed = true;
	if (readLen < 0) {
		server.requestDelete(this);
		return;
	};

	inBuffer.write(buffer, readLen);

	if (this->_transactions.empty()) {
		this->_transactions.push(new HttpTransaction());
	}

	while (inBuffer.peek() != std::stringstream::traits_type::eof()) {
		if (this->_transactions.back()->recvRequest(inBuffer, server)) {
			this->_transactions.push(new HttpTransaction());
		}
	}

	if (closed) this->_transactions.back()->isLast(true);

	if (this->canHandleEpollOut()) {
		server.epoll().mod(*this);
	}
}

void ClientSocket::onEpollOut(WebServer &webServer) {
	HttpTransaction *transaction = this->_transactions.front();
	if (transaction->sendResponse(*this)) {
		if (transaction->isLast()) webServer.requestDelete(this);
		delete transaction;
		this->_transactions.pop();
	}
	if (!this->canHandleEpollOut()) {
		webServer.epoll().mod(*this);
	}
}
