#include "ListeningSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

ListeningSocket::~ListeningSocket() {}

ListeningSocket::ListeningSocket(int socketFd, const struct sockaddr_storage &address) : ASocket(socketFd) {

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

ListeningSocket ListeningSocket::createListeningSocket(const struct sockaddr_storage &address) {
    const int socketFd = socket(address.ss_family, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd < 0) {
        throw std::runtime_error()
    }

	int opt = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(socketFd, reinterpret_cast<const struct sockaddr *>(&address), sizeof(address)) < 0) {
		throw std::runtime_error("[FATAL_ERROR] Bind failed: " + std::string(strerror(errno)));
	}

	if (listen(socketFd, SOMAXCONN) < 0) {
		throw std::runtime_error("[FATAL_ERROR] Listen failed: " + std::string(strerror(errno)));
	}
}
