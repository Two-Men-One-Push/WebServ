#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <iostream>
#include <ostream>
#include <string>

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
	delete this->_cgiInterface;
}

/**
 * This behavior is described by the RFC depending of the version
 */
bool HttpResponse::hasBody() const {
	if (this->_version == HTTP1_0) {
		return true;
	} else {
		/** @see https://datatracker.ietf.org/doc/html/rfc9112#section-6.3-2.1 first case talking about response messages (aka HTTP response) */
		if (HttpStatus::isInformational(this->_status) || this->_status == HttpStatus::NoContent || this->_status == HttpStatus::NotModified)
			return false;
		return this->HttpMessage::hasBody();
	}
}

HttpStatus::Code HttpResponse::status() const {
	return this->_status;
}

std::ostream &HttpResponse::printTypeInfo(std::ostream &os) const {
	return os << this->versionStr() << " " << this->_status << '\n';
}
