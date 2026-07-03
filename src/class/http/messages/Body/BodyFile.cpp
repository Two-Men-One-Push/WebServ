#include "./BodyFile.hpp"
#include "File/File.hpp"
#include <cstddef>
#include <sys/types.h>

BodyFile::BodyFile(const std::string &path, int flags, mode_t mode) : File(path, flags, mode) {}

BodyFile::~BodyFile() {}

ssize_t BodyFile::read(char *buff, size_t n) {
	return this->File::read(buff, n);
}

ssize_t BodyFile::write(const char *buff, size_t n) {
	return this->File::write(buff, n);
}
