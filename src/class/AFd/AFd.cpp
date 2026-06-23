#include "./AFd.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

AFd::AFd(int fd) : _fd(fd) {
	fcntl(fd, F_SETFD, FD_CLOEXEC);
}

AFd::~AFd() {
	close(_fd);
}

int AFd::fd() const {
	return _fd;
}

ssize_t AFd::read(void *buf, size_t len) const {
	return ::read(this->_fd, buf, len);
}

ssize_t AFd::recv(void *buf, size_t len, int flags) const {
	return ::recv(this->_fd, buf, len, flags);
}

ssize_t AFd::write(const void *buf, size_t len) const {
	return ::write(this->_fd, buf, len);
}

ssize_t AFd::send(const void *buf, size_t len, int flags) const {
	return ::send(this->_fd, buf, len, flags);
}

int AFd::dup2(int fd) const {
	return ::dup2(this->_fd, fd);
}
