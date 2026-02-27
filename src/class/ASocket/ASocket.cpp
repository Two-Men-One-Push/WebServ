#include "ASocket.hpp"
#include "errors/WebservErrors.hpp"
#include <cerrno>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

ASocket::ASocket(int fd) : AFd(fd) {
	const int baseFlags = fcntl(fd, F_GETFL, 0);
	if (baseFlags < 0) {
		throw webserv_errors::SysError("fcntl", errno, "Getting flags");
	}
	if (baseFlags & O_NONBLOCK) return;
	if (fcntl(fd, F_SETFL, baseFlags | O_NONBLOCK) < 0) {
		throw webserv_errors::SysError("fcntl", errno, "Setting flags");
	}
}

ASocket::~ASocket() {}
