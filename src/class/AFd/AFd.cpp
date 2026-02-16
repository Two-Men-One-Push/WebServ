#include "AFd/AFd.hpp"
#include <stdexcept>
#include <unistd.h>

AFd::AFd(int fd) : _fd(fd) {
	if (_fd < 0) {
		throw std::invalid_argument("Invalid FD");
	}
}

AFd::~AFd() {
	if (close(_fd)) {
		
	}
}
