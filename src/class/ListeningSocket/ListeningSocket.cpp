#include "ListeningSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>

ListeningSocket::ListeningSocket(int socketFd, const struct sockaddr_storage &address) : ASocket(socketFd) {
	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(_fd, reinterpret_cast<const struct sockaddr *>(&address), sizeof(address)) < 0) {
		throw webserv_errors::SysError("bind", errno);
	}

	if (listen(_fd, SOMAXCONN) < 0) {
		throw webserv_errors::SysError("listen", errno);
	}
}

ListeningSocket::~ListeningSocket() {}

ClientSocket *ListeningSocket::acceptConnexion(void) const {
	return new ClientSocket(ClientSocket::createFromListener(_fd));
}

ListeningSocket ListeningSocket::create(const struct sockaddr_storage &address) {
	const int socketFd = socket(address.ss_family, SOCK_STREAM, IPPROTO_TCP);
	if (socketFd < 0) {
		throw webserv_errors::SysError("socket", errno);
	}

	return ListeningSocket(socketFd, address);
}
