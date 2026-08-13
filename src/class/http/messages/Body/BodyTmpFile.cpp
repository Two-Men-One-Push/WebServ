#include "./BodyTmpFile.hpp"
#include "File/TmpFile.hpp"
#include <cstddef>
#include <sys/types.h>

BodyTmpFile::BodyTmpFile(const std::string &path, int flags, mode_t mode) : TmpFile(path, flags, mode) {}

BodyTmpFile::~BodyTmpFile() {}

ssize_t BodyTmpFile::read(char *buff, size_t n) {
	return this->TmpFile::read(buff, n);
}

ssize_t BodyTmpFile::write(const char *buff, size_t n) {
	return this->TmpFile::write(buff, n);
}
