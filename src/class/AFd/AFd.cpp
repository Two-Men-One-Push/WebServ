#include "./AFd.hpp"
#include <unistd.h>

AFd::AFd(int fd) : _fd(fd) {}

AFd::~AFd() {
	close(_fd);
}
