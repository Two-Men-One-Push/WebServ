#include "./WebservErrors.hpp"
#include <cstring>

using namespace webserv_errors;

SysError::SysError(int err) : _err(err), _context(""), _cause(""), _msg(strerror(err)) {
}

SysError::SysError(const std::string &context, int err)
	: _err(err), _context(context), _cause(""), _msg(context + ": " + strerror(err)) {}

SysError::SysError(const std::string &context, int err, const std::string &cause)
	: _err(err), _context(context), _cause(cause), _msg(context + ": " + strerror(err) + ": " + cause) {}

SysError::~SysError() throw() {}

const char *SysError::what() const throw() {
	return _msg.c_str();
}

int SysError::getErr() const {
	return _err;
}
