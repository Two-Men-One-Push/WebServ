#ifndef HTTPSTANDARDEXCEPTION_HPP
#define HTTPSTANDARDEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include "http/errors/HttpException.hpp"

template <HttpStatus::Code statusT>
class HttpExceptionT : public HttpException {
  public:
	HttpExceptionT() throw() : HttpException(statusT) {}

	HttpExceptionT(const std::string &detail) throw() : HttpException(statusT, detail) {}

	virtual ~HttpExceptionT() throw() {}
};

#define HTTP_EXCEPTION(Name, Code)                                                            \
	class Name : public HttpExceptionT<HttpStatus::Code> {                                    \
	  public:                                                                                 \
		Name() throw() {}                                                                     \
		Name(const std::string &detail) throw() : HttpExceptionT<HttpStatus::Code>(detail) {} \
	};

namespace HttpExceptions {
HTTP_EXCEPTION(NoContentException, NoContent)
HTTP_EXCEPTION(BadRequestException, BadRequest)
HTTP_EXCEPTION(NotImplementedException, NotImplemented)
HTTP_EXCEPTION(HTTPVersionNotSupportedException, HTTPVersionNotSupported)
HTTP_EXCEPTION(URITooLongException, URITooLong)
HTTP_EXCEPTION(InternalServerErrorException, InternalServerError)
}; // namespace HttpExceptions

#endif
