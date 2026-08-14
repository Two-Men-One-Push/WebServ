#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/IBody.hpp"
#include "http/messages/HttpMessage.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>

size_t HttpResponse::_maxVersionSize = HTTP_BUFFER_SIZE;
size_t HttpResponse::_maxStatusSize = 3;
size_t HttpResponse::_maxMessageSize = HTTP_BUFFER_SIZE;

HttpResponse::HttpResponse()
	: HttpMessage(),
	  _status(HttpStatus::NoStatus),
	  _message(),
	  _cgiInterface(NULL) {}

HttpResponse::HttpResponse(const HttpResponse &other)
	: HttpMessage(other),
	  _status(other._status),
	  _message(other._message),
	  _cgiInterface(other._cgiInterface) {}

// HttpResponse &HttpResponse::operator=(const HttpResponse &other) {
// 	if (this != &other) {
// 		this->HttpMessage::operator=(other);
// 		this->_status = other._status;
// 		this->_body = other._body;
// 	}
// 	return *this;
// }

HttpResponse::~HttpResponse() {
	if (this->_cgiInterface) {
		this->_cgiInterface->requestDelete();
	}
}

HttpStatus::Code HttpResponse::status() const {
	return this->_status;
}

std::ostream &HttpResponse::printTypeInfo(std::ostream &os) const {
	return os << this->versionStr() << " " << this->_status << '\n';
}
