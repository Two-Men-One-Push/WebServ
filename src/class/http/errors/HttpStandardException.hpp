#ifndef HTTPSTANDARDEXCEPTION_HPP
#define HTTPSTANDARDEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include "http/errors/HttpException.hpp"

template <HttpStatus::Code statusT>
class HttpExceptionT : public HttpException {
  public:
	HttpExceptionT() throw() : HttpException(statusT) {}

	virtual ~HttpExceptionT() throw() {}
};

namespace HttpExceptions {
class NoContentException : public HttpExceptionT<HttpStatus::NoContent> {};

class BadRequestException : public HttpExceptionT<HttpStatus::BadRequest> {};

class NotImplementedException : public HttpExceptionT<HttpStatus::NotImplemented> {};

class HTTPVersionNotSupportedException : public HttpExceptionT<HttpStatus::HTTPVersionNotSupported> {};

class URITooLongException : public HttpExceptionT<HttpStatus::URITooLong> {};
}; // namespace HttpExceptions

#endif
