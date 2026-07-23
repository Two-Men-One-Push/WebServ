#include "ASocket.hpp"
#include "EpollInstance/EpollWatchable.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ASocket::ASocket(int fd, const sockaddr &address, socklen_t addressLen) : AEpollWatchable(fd) {
	sockaddr_storage *thisAddressPtr = &this->_address;
	std::memcpy(thisAddressPtr, &address, addressLen);

	const int baseFlags = fcntl(fd, F_GETFL, 0);
	if (baseFlags < 0) {
		throw WebservErrors::SysError("fcntl", errno, "Getting flags");
	}
	if (baseFlags & O_NONBLOCK) return;
	if (fcntl(fd, F_SETFL, baseFlags | O_NONBLOCK) < 0) {
		throw WebservErrors::SysError("fcntl", errno, "Setting flags");
	}
}

ASocket::ASocket(int fd, const sockaddr_storage &address, socklen_t addressLen) : AEpollWatchable(fd) {
	sockaddr_storage *thisAddressPtr = &this->_address;
	std::memcpy(thisAddressPtr, &address, addressLen);

	const int baseFlags = fcntl(fd, F_GETFL, 0);
	if (baseFlags < 0) {
		throw WebservErrors::SysError("fcntl", errno, "Getting flags");
	}
	if (baseFlags & O_NONBLOCK) return;
	if (fcntl(fd, F_SETFL, baseFlags | O_NONBLOCK) < 0) {
		throw WebservErrors::SysError("fcntl", errno, "Setting flags");
	}
}

int ASocket::createFdFromListener(const ListeningSocket &listener, struct sockaddr_storage &outClientAddress) {
	socklen_t addrLen = sizeof(outClientAddress);

	const int fd = listener.accept(reinterpret_cast<struct sockaddr *>(&outClientAddress), &addrLen);

	if (fd < 0) throw WebservErrors::SysError("accept", errno);

	return fd;
}

ASocket::ASocket(const ListeningSocket &listeningSocket)
	: AEpollWatchable(ASocket::createFdFromListener(listeningSocket, this->_address)) {}

ASocket::~ASocket() {}
