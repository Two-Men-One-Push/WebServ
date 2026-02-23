#include "ListeningSocket.hpp"
#include "ASocket/ASocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

ListeningSocket::ListeningSocket(int socketFd, const sockaddr &address, socklen_t addressLen) : ASocket(socketFd) {
	int opt = 1;
	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(_fd, &address, addressLen) < 0) {
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

ListeningSocket ListeningSocket::create(const sockaddr &addr, socklen_t addresslen) {


	const int socketFd = socket(addr.sa_family, SOCK_STREAM, 0);

	if (socketFd < 0) {
		throw webserv_errors::SysError("socket", errno);
	}

	return ListeningSocket(socketFd, addr, addresslen);
}
