#ifndef HTTPSTANDARDEXCEPTION_HPP
#define HTTPSTANDARDEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include "http/errors/HttpErrors.hpp"

template <HttpStatus::Code statusT>
class HttpExceptionT : public HttpError {
  public:
	HttpExceptionT() throw() : HttpError(statusT) {}

	HttpExceptionT(const std::string &detail) throw() : HttpError(statusT, detail) {}

	virtual ~HttpExceptionT() throw() {}
};

#define HTTP_EXCEPTION(Name, Code)                                                            \
	class Name : public HttpExceptionT<HttpStatus::Code> {                                    \
	  public:                                                                                 \
		Name() throw() {}                                                                     \
		Name(const std::string &detail) throw() : HttpExceptionT<HttpStatus::Code>(detail) {} \
	};

namespace HttpErrors {
HTTP_EXCEPTION(NoContentException, NoContent)

HTTP_EXCEPTION(BadRequestException, BadRequest)
HTTP_EXCEPTION(ForbiddenException, Forbidden)
HTTP_EXCEPTION(NotFoundException, NotFound)
HTTP_EXCEPTION(ConflictException, Conflict)
HTTP_EXCEPTION(URITooLongException, URITooLong)

HTTP_EXCEPTION(InternalServerErrorException, InternalServerError)
HTTP_EXCEPTION(NotImplementedException, NotImplemented)
HTTP_EXCEPTION(BadGatewayException, BadGateway)
HTTP_EXCEPTION(HTTPVersionNotSupportedException, HTTPVersionNotSupported)
}; // namespace HttpErrors

#endif
