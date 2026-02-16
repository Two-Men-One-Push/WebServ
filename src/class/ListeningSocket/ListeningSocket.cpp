#include "ListeningSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

ListeningSocket::~ListeningSocket() {}

ListeningSocket::ListeningSocket(const struct sockaddr_storage &address) try : ASocket(socket(address.ss_family, SOCK_STREAM, IPPROTO_TCP)) {
	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(_fd, reinterpret_cast<const struct sockaddr *>(&address), sizeof(address)) < 0) {
		throw std::runtime_error("[FATAL_ERROR] Bind failed: " + std::string(strerror(errno)));
	}

	if (listen(_fd, SOMAXCONN) < 0) {
		throw std::runtime_error("[FATAL_ERROR] Listen failed: " + std::string(strerror(errno)));
	}
} catch (const std::invalid_argument &exception) {

	throw std::runtime_error("[FATAL_ERROR] Socket failed: " + std::string(strerror(errno)));
}

ClientSocket *ListeningSocket::acceptConnexion(void) const {
	int fd;
	struct sockaddr_storage address;
	socklen_t addressLen;

	addressLen = sizeof(address);
	try {
		fd = accept(this->_fd, reinterpret_cast<struct sockaddr *>(&address), &addressLen);
		return new ClientSocket(fd, address, addressLen);
	} catch (const std::invalid_argument &exception) {
		std::cerr << "[ERROR] Accept failed" << std::endl;
		return NULL;
	}
}

int ListeningSocket::getFd() const {
	return this->_fd;
}
