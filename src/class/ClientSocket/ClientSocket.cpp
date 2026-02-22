#include "./ClientSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <sys/socket.h>

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen) : ASocket(fd), _address(address), _addressLen(addressLen) {}

ClientSocket::~ClientSocket() {}

ClientSocket ClientSocket::createFromListener(int listenerFd) {
	struct sockaddr_storage clientAddr;
	socklen_t addrLen = sizeof(clientAddr);

	const int fd = accept(listenerFd, reinterpret_cast<struct sockaddr*>(&clientAddr), &addrLen);

	if (fd < 0) {
		throw webserv_errors::SysError("accept", errno);
	}

	return ClientSocket(fd, clientAddr, addrLen);
}

const struct sockaddr_storage &ClientSocket::getAdress() const {
	return _address;
}

socklen_t ClientSocket::getAdressLen() const {
	return _addressLen;
}
