#include "./WebservErrors.hpp"
#include <cstring>

using namespace WebservErrors;

Runtime::Runtime(const std::string &message) throw()
	: _message(message) {}

Runtime::Runtime(const Runtime &other) throw()
	: _message(other._message) {}

Runtime::~Runtime() throw() {}

const char *Runtime::what() const throw() {
	return _message.c_str();
}
