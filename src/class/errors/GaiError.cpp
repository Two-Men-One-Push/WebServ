#include "./WebservErrors.hpp"
#include <netdb.h>

using namespace WebservErrors;

GaiError::GaiError(int err) throw()
	: _err(err), _context(""), _cause(""), _message(gai_strerror(err)) {}

GaiError::GaiError(const std::string &context, int err) throw()
	: _err(err), _context(context), _cause(""), _message(context + ": " + gai_strerror(err)) {}

GaiError::GaiError(const std::string &context, int err, const std::string &cause) throw()
	: _err(err), _context(context), _cause(cause), _message(context + ": " + gai_strerror(err) + ": " + cause) {}

GaiError::GaiError(const GaiError &other) throw()
	: _err(other._err), _context(other._context), _cause(other._cause), _message(other._message) {}

GaiError::~GaiError() throw() {}

const char *GaiError::what() const throw() {
	return _message.c_str();
}

int GaiError::getErr() const {
	return _err;
}
