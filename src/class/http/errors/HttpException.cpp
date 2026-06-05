#include "./HttpException.hpp"
#include "http/HttpStatus.hpp"

HttpException::HttpException(HttpStatus::Code status) throw()
	: _message(HttpStatus::reasonPhrase(status)),
	  _status(status) {}

HttpException::~HttpException() throw() {}

const char *HttpException::what() const throw() { return _message.c_str(); }

HttpStatus::Code HttpException::status() const { return _status; }

const std::string &HttpException::message() const { return _message; }
