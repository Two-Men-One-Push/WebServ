#include "./WebServer.hpp"
#include "AFd/AFd.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "config/MainContext/MainContext.hpp"
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

WebServer::WebServer(Config &config) : _config(config), _epoll(EpollInstance::create()) {
	/* temp */
	short port = 6969;
	std::string ipAddress = "localhost";

	_listeningSockets.reserve(10 /* count server and the number of different listen directives and reserve enough */);
	/* temp */

	struct addrinfo hints;
	struct addrinfo *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	std::stringstream oss;
	oss << port;

	const int ret = getaddrinfo(ipAddress.c_str(), oss.str().c_str(), &hints, &res);

	if (ret) {
		throw std::runtime_error(gai_strerror(ret));
	}

	addrinfo *currentAddressInfo = res;
	while (currentAddressInfo) {
		_listeningSockets.push_back(ListeningSocket::createNew(*currentAddressInfo->ai_addr, currentAddressInfo->ai_addrlen));
		_epoll.registerFd(*_listeningSockets.back());
		currentAddressInfo = currentAddressInfo->ai_next;
	}

	freeaddrinfo(res);

	std::cout << "Listening http://" << ipAddress << ":" << port << std::endl;

	while (true) {
		std::vector<EpollEvent> events;
		_epoll.wait(events);
		for (std::vector<EpollEvent>::iterator it = events.begin(); it != events.end(); ++it) {
			it->fd->handleEvents(it->events, *this);
		}

		this->deleteClientSockets();
		// char buffer[4096];

		// errno = 0;
		// while (errno != EAGAIN) {
		// 	ssize_t readLen = read(test.getFd(), buffer, 4096);
		// 	std::cout.write(buffer, readLen);
		// }
	}
}

WebServer::~WebServer() {}

void WebServer::addClient(ClientSocket *client) {
	_clientSockets.push_back(client);
	_epoll.registerFd(*client);
}

void WebServer::updateFd(AFd &fd) {
	this->_epoll.updateFd(fd);
}

void WebServer::requestDelete(ClientSocket *client) {
	this->_clientSocketsToDelete.push_back(client);
}

void WebServer::deleteClientSockets() {
	for (std::vector<ClientSocket*>::iterator dit = this->_clientSocketsToDelete.begin(); dit != _clientSocketsToDelete.end(); ++dit) {
		ClientSocket *cs = *dit;
		for (std::vector<ClientSocket *>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); ++it) {
			if (*it == cs) {
				_clientSockets.erase(it);
				break;
			}
		}
		delete cs;
	}
	_clientSocketsToDelete.clear();
}
