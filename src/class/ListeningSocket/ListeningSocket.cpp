#include "./ListeningSocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "model/Server/Server.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

int ListeningSocket::createFd(const sockaddr &addr) {
	const int socketFd = socket(addr.sa_family, SOCK_STREAM, 0);

	if (socketFd < 0) {
		throw WebservErrors::SysError("socket", errno);
	}

	return socketFd;
}

ListeningSocket::ListeningSocket(const sockaddr &address, socklen_t addressLen, const Server &serverConfig)
	: ASocket(ListeningSocket::createFd(address), address, addressLen), _serverConfig(serverConfig) {


	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(_fd, &address, addressLen) < 0) {
		throw WebservErrors::SysError("bind", errno);
	}

	if (listen(_fd, SOMAXCONN) < 0) {
		throw WebservErrors::SysError("listen", errno);
	}
}

ListeningSocket::~ListeningSocket() {}

ClientSocket *ListeningSocket::acceptConnexion(void) const {
	return ClientSocket::createFromListener(*this, this->_serverConfig);
}

u_int32_t ListeningSocket::getHandledEvents() const {
	return EPOLLIN;
}

void ListeningSocket::handleEvents(u_int32_t events, WebServer &webServer) {
	if (events & EPOLLIN) {
		this->onEpollIn(webServer);
	} else if (events & EPOLLHUP) {
		std::cout << "Unhandled event : EPOLLHUP" << std::endl;
	} else if (events & EPOLLERR) {
		std::cout << "Unhandled event : EPOLLERR" << std::endl;
	} else {
		std::cout << "Unhandled event :" << events << std::endl;
	}
}

void ListeningSocket::onEpollIn(WebServer &webServer) const {
	webServer.addClient(this->acceptConnexion());
}

int ListeningSocket::accept(struct sockaddr *address, socklen_t *addressLen) const {
	return ::accept(this->_fd, address, addressLen);
}
