#include "./AFd.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

	AFd::AFd(int fd) : _fd(fd) {
}

AFd::~AFd() {
	close(_fd);
}

int AFd::fd() const {
	return _fd;
}

ssize_t AFd::write(const void *buf, size_t len) {
	return ::write(this->_fd, buf, len);
}

ssize_t AFd::send(const void *buf, size_t len, int flags) {
	return ::send(this->_fd, buf, len, flags);
}
