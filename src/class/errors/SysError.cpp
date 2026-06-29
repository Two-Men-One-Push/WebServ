#include "./WebservErrors.hpp"
#include <cstring>

using namespace WebservErrors;

SysError::SysError(int err) throw()
	: _err(err), _context(""), _cause(""), _message(strerror(err)) {}

SysError::SysError(const std::string &context, int err) throw()
	: _err(err), _context(context), _cause(""), _message(context + ": " + strerror(err)) {}

SysError::SysError(const std::string &context, int err, const std::string &cause) throw()
	: _err(err), _context(context), _cause(cause), _message(context + ": " + strerror(err) + ": " + cause) {}

SysError::~SysError() throw() {}

const char *SysError::what() const throw() {
	return _message.c_str();
}

int SysError::getErr() const {
	return _err;
}
