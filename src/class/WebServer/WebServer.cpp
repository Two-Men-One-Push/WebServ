#include "./WebServer.hpp"
#include "Fd/Fd.hpp"
#include "CGI/CGIInterface.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

WebServer::WebServer() : _epoll() {
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
		throw WebservErrors::GaiError("getaddrinfo", ret, ipAddress + " " + oss.str());
	}

	for (addrinfo *currentAddressInfo = res; currentAddressInfo != NULL; currentAddressInfo = currentAddressInfo->ai_next) {
		try {
			_listeningSockets.push_back(ListeningSocket::createNew(*currentAddressInfo->ai_addr, currentAddressInfo->ai_addrlen));
		} catch (...) {
			freeaddrinfo(res);
			throw;
		}
		_epoll.add(*_listeningSockets.back());
	}

	freeaddrinfo(res);

	std::cout << "Listening http://" << ipAddress << ":" << port << ' ' << std::endl;

	// HttpTransaction testTransaction;
	// CGIInterface test("./www/cgi/test-test.py", testTransaction, *this);
	// CGIInterface test("/bin/cat", testTransaction, *this);

	while (true) {
		std::vector<EpollEvent> events;
		_epoll.wait(events);
		for (std::vector<EpollEvent>::iterator it = events.begin(); it != events.end(); ++it) {
			it->fd->handleEvents(it->events, *this);
		}

		for (std::vector<ClientSocket *>::const_iterator it = this->_clientSockets.begin(); it != this->_clientSockets.end(); ++it) {
			this->_epoll.mod(**it);
		}
		this->deleteClientSockets();
	}
}

WebServer::~WebServer() {}

void WebServer::addClient(ClientSocket *client) {
	_clientSockets.push_back(client);
	_epoll.add(*client);
}

const EpollInstance &WebServer::epoll() const {
	return this->_epoll;
}

void WebServer::requestDelete(ClientSocket *client) {
	std::vector<ClientSocket *> &deleteList = this->_clientSocketsToDelete;
	if (std::find(deleteList.begin(), deleteList.end(), client) == deleteList.end()) {
		deleteList.push_back(client);
	}
}

void WebServer::deleteClientSockets() {
	for (std::vector<ClientSocket *>::iterator dit = this->_clientSocketsToDelete.begin(); dit != _clientSocketsToDelete.end(); ++dit) {
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
