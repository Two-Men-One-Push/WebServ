#include "ASocket.hpp"
#include <fcntl.h>
#include <stdexcept>
#include <sys/types.h>
#include <unistd.h>

ASocket::~ASocket() {
	close(_fd);
}

ASocket::ASocket(int fd) : AFd(fd) {
	if (fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL, 0) | O_NONBLOCK) < 0) {
		close(_fd);
		throw std::runtime_error("Fcntl failed to set non-blocking");
	}
}
