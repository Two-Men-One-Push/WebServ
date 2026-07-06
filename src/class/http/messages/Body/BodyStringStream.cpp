#include "./BodyStringStream.hpp"
#include <sstream>

BodyStringStream::BodyStringStream() : std::stringstream() {}

BodyStringStream::~BodyStringStream() {}

ssize_t BodyStringStream::read(char *buf, size_t size) {
	this->std::stringstream::read(buf, size);
	return this->std::stringstream::gcount();
}

ssize_t BodyStringStream::write(const char *buf, size_t size) {
	this->std::stringstream::write(buf, size);
	return size;
}
