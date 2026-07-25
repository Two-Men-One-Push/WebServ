#include "./WebServer.hpp"
#include "CGI/CGIInterface.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "Fd/Fd.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "errors/WebservErrors.hpp"
#include "model/Config/Config.hpp"
#include "model/Server/Server.hpp"
#include <algorithm>
#include <csignal>
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

WebServer::WebServer(const Config &config) : _epoll() {
	::signal(SIGINT, WebServer::sigintHandler);
	this->startListeningSockets(config);

	while (WebServer::running) {
		std::vector<EpollEvent> events;
		try {
			_epoll.wait(events);
		} catch (const WebservErrors::SysError &e) {
			if (e.err() != EINTR && WebServer::signal != SIGINT) throw;
		}
		for (std::vector<EpollEvent>::iterator it = events.begin(); it != events.end(); ++it) {
			it->fd->handleEvents(it->events, *this);
		}

		for (std::vector<ClientSocket *>::const_iterator it = this->_clientSockets.begin(); it != this->_clientSockets.end(); ++it) {
			this->_epoll.mod(**it);
		}
		this->deleteClientSockets();
	}
}

WebServer::~WebServer() {
	std::vector<ListeningSocket *> &listeningSockets = this->_listeningSockets;
	for (std::vector<ListeningSocket *>::iterator it = listeningSockets.begin(); it != listeningSockets.end(); ++it) {
		delete *it;
	}
	std::vector<ClientSocket *> &clientSockets = this->_clientSockets;
	for (std::vector<ClientSocket *>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
		delete *it;
	}
}

void WebServer::startListeningSockets(const Config &config) {
	const std::vector<Server> &serverConfigs = config.http().servers();

	struct addrinfo hints;
	struct addrinfo *res;
	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;

	for (std::vector<Server>::const_iterator sit = serverConfigs.begin(); sit != serverConfigs.end(); ++sit) {
		const std::vector<std::pair<std::string, int> > &listens = sit->listen();
		for (std::vector<std::pair<std::string, int> >::const_iterator lit = listens.begin(); lit != listens.end(); ++lit) {
			std::stringstream oss;
			oss << lit->second;

			const int ret = getaddrinfo(lit->first.c_str(), oss.str().c_str(), &hints, &res);

			if (ret) {
				throw WebservErrors::GaiError("getaddrinfo", ret, lit->first + " " + oss.str());
			}

			for (addrinfo *currentAddressInfo = res; currentAddressInfo != NULL; currentAddressInfo = currentAddressInfo->ai_next) {
				try {
					_listeningSockets.push_back(new ListeningSocket(*currentAddressInfo->ai_addr, currentAddressInfo->ai_addrlen, *sit));
				} catch (...) {
					freeaddrinfo(res);
					throw;
				}
				_epoll.add(*_listeningSockets.back());
			}

			freeaddrinfo(res);

			std::cout << "Listening http://" << lit->first << ":" << lit->second << ' ' << std::endl;
		}
	}
}

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
