#include "./File.hpp"
#include "Fd/Fd.hpp"
#include "errors/WebservErrors.hpp"
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>

File::File(int fd) : Fd(fd) {}
File::File(const std::string &path, int flags, mode_t mode) : Fd(File::openFd(path, flags, mode)) {}

File::~File() {}

int File::openFd(const std::string &path, int flags, mode_t mode) {
	int fd;

	if (flags & O_CREAT || flags & O_TMPFILE) {
		fd = ::open(path.c_str(), flags, mode);
	} else {
		fd = ::open(path.c_str(), flags);
	}

	if (fd < 0) throw WebservErrors::SysError("open", errno, path);
	return fd;
}
