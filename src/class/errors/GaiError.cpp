#include "./WebservErrors.hpp"
#include <netdb.h>

using namespace webserv_errors;

GaiError::GaiError(int err) : _err(err), _context(""), _cause(""), _msg(gai_strerror(err)) {
}

GaiError::GaiError(const std::string &context, int err)
	: _err(err), _context(context), _cause(""), _msg(context + ": " + gai_strerror(err)) {}

GaiError::GaiError(const std::string &context, int err, const std::string &cause)
	: _err(err), _context(context), _cause(cause), _msg(context + ": " + gai_strerror(err) + ": " + cause) {}

GaiError::~GaiError() throw() {}

const char *GaiError::what() const throw() {
	return _msg.c_str();
}

int GaiError::getErr() const {
	return _err;
}
