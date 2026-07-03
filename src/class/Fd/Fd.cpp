#include "./Fd.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Fd::Fd(int fd) : _fd(fd) {
	fcntl(fd, F_SETFD, FD_CLOEXEC);
}

Fd::~Fd() {
	close(_fd);
}

int Fd::fd() const {
	return _fd;
}

ssize_t Fd::read(void *buf, size_t len) const {
	return ::read(this->_fd, buf, len);
}

ssize_t Fd::recv(void *buf, size_t len, int flags) const {
	return ::recv(this->_fd, buf, len, flags);
}

ssize_t Fd::write(const void *buf, size_t len) const {
	return ::write(this->_fd, buf, len);
}

ssize_t Fd::send(const void *buf, size_t len, int flags) const {
	return ::send(this->_fd, buf, len, flags);
}

int Fd::dup2(int fd) const {
	return ::dup2(this->_fd, fd);
}
