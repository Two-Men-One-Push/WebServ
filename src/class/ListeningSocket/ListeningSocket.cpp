#include "./ListeningSocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "model/Server/Server.hpp"
#include <cerrno>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

ListeningSocket::ListeningSocket(int socketFd, const sockaddr &address, socklen_t addressLen, const Server &serverConfig)
	: ASocket(socketFd), _serverConfig(serverConfig) {
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
	return ClientSocket::createFromListener(_fd, this->_serverConfig);
}

ListeningSocket ListeningSocket::create(const sockaddr &addr, socklen_t addresslen, const Server &serverConfig) {

	const int socketFd = socket(addr.sa_family, SOCK_STREAM, 0);

	if (socketFd < 0) {
		throw WebservErrors::SysError("socket", errno);
	}

	return ListeningSocket(socketFd, addr, addresslen, serverConfig);
}

ListeningSocket *ListeningSocket::createNew(const sockaddr &addr, socklen_t addresslen, const Server &serverConfig) {

	const int socketFd = socket(addr.sa_family, SOCK_STREAM, 0);

	if (socketFd < 0) {
		throw WebservErrors::SysError("socket", errno);
	}

	return new ListeningSocket(socketFd, addr, addresslen, serverConfig);
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
