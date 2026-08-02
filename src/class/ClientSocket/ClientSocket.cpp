#include "./ClientSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "WebServer/WebServer.hpp"
#include "http/HttpTransaction.hpp"
#include <cerrno>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sstream>
#include <stdint.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ClientSocket::ClientSocket(const ListeningSocket &listeningSocket)
	: ASocket(listeningSocket),
	  _serverConfig(listeningSocket.serverConfig()),
	  _serverAddress(listeningSocket.address()),
	  _inClosed(false),
	  _outBuffer(),
	  _transactions() {}

ClientSocket::~ClientSocket() {
	while (!this->_transactions.empty()) {
		delete this->_transactions.front();
		this->_transactions.pop();
	}
}

const struct sockaddr_storage &ClientSocket::address() const {
	return _address;
}

uint32_t ClientSocket::getHandledEvents() const {
	uint32_t result = 0;
	if (!this->_inClosed)
		result |= EPOLLIN;
	if (this->canHandleEpollOut())
		result |= EPOLLOUT;
	return result;
}

bool ClientSocket::canHandleEpollOut() const {
	return !this->_transactions.empty() && this->_transactions.front()->response().inCompleted();
}

void ClientSocket::handleEvents(u_int32_t events, WebServer &webServer) {
	if (events & (EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLERR)) {
		if (events & EPOLLHUP || events & EPOLLERR) {
			webServer.requestDelete(this);
		} else {
			if (events & EPOLLIN) {
				this->onEpollIn(webServer);
			}
			if (events & EPOLLOUT) {
				this->onEpollOut(webServer);
			}
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

	if (!readLen) this->_inClosed = true;
	if (readLen < 0) {
		server.requestDelete(this);
		return;
	};

	// std::cerr << "\e[0;31m";
	// std::cerr.write(buffer, readLen);
	// std::cerr << "\e[0m\n";
	inBuffer.write(buffer, readLen);

	if (this->_transactions.empty()) {
		this->_transactions.push(new HttpTransaction(this->_serverConfig, this->_serverAddress, this->_address));
	}

	while (inBuffer.peek() != std::stringstream::traits_type::eof()) {
		if (this->_transactions.back()->request().inCompleted()) {
			this->_transactions.push(new HttpTransaction(this->_serverConfig, this->_serverAddress, this->_address));
		}
		if (this->_transactions.back()->recvRequest(inBuffer, server)) {
			if (!this->_transactions.back()->keepAlive()) {
				// If the request parsing is completed but the response will close the connection
				this->_inClosed = true;
				break;
			}
		} else if (this->_inClosed) {
			// If the request parsing isn't completed but the input is closed (may lead to errors or not, it will depend of when the connection closed)
			this->_transactions.back()->closeRequestInput();
		}
	}

	if (this->_inClosed) this->_transactions.back()->kill();

	if (this->canHandleEpollOut()) {
		server.epoll().mod(*this);
	}
}

void ClientSocket::onEpollOut(WebServer &webServer) {
	HttpTransaction *transaction = this->_transactions.front();
	bool sendCompleted;
	try {
		sendCompleted = transaction->sendResponse(*this);
	} catch (...) {
		webServer.requestDelete(this);
		return;
	}
	if (sendCompleted) {
		if (!transaction->keepAlive()) {
			webServer.requestDelete(this);
		}
		delete transaction;
		this->_transactions.pop();
	}
	if (!this->canHandleEpollOut()) {
		webServer.epoll().mod(*this);
	}
}
