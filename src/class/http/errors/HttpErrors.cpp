#include "./HttpErrors.hpp"
#include "http/HttpStatus.hpp"

HttpError::HttpError(HttpStatus::Code status) throw()
	: _message(HttpStatus::reasonPhrase(status)),
	  _status(status) {}

HttpError::HttpError(HttpStatus::Code status, const std::string &detail) throw()
	: _message(HttpStatus::reasonPhrase(status) + ": " + detail),
	  _status(status) {}

HttpError::~HttpError() throw() {}

const char *HttpError::what() const throw() { return _message.c_str(); }

HttpStatus::Code HttpError::status() const { return _status; }

const std::string &HttpError::message() const { return _message; }
