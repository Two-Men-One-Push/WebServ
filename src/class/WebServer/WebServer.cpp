#include "./WebServer.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "config/MainContext/MainContext.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>

WebServer::WebServer(Config &config) : _config(config), _epoll(EpollInstance::create()) {

	/* temp */
	short port = 6969;
	std::string ipAddress = "127.0.0.1";

	_listeningSockets.reserve(10 /* count server and the number of different listen directivesand reserve enough */);
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
		_listeningSockets.push_back(ListeningSocket::create(*currentAddressInfo->ai_addr, currentAddressInfo->ai_addrlen));
		currentAddressInfo = currentAddressInfo->ai_next;
	}
	std::cout << "Listening on " << ipAddress << ":" << port << std::endl;

	while (true) {
		AFd &test = _epoll.wait();

		char buffer[4096];

		errno = 0;
		while (errno != EAGAIN) {
			ssize_t readLen = recv(test.getFd(), buffer, 4096, 0);
			std::cout.write(buffer, readLen);
		}
	}
}

WebServer::~WebServer() {}
