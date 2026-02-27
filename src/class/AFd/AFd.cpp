#include "./AFd.hpp"
#include <unistd.h>

AFd::AFd(int fd) : _fd(fd) {}

AFd::~AFd() {
	close(_fd);
}

int AFd::getFd() const {
	return _fd;
}
