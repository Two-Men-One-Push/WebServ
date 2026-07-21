#include "ASocket.hpp"
#include "EpollInstance/EpollWatchable.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <cstring>
#include <fcntl.h>
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

ASocket::~ASocket() {}
