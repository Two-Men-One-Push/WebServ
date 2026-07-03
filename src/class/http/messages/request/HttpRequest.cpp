#include "./HttpRequest.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <utility>

// cleanest way to define initialised map in cpp98

static std::pair<std::string, HttpMethod> pairs[] = {
	std::make_pair("GET", GET),
	std::make_pair("HEAD", HEAD),
	std::make_pair("POST", POST),
	std::make_pair("DELETE", DELETE),
};

static int httpMethodCount = sizeof(pairs) / sizeof(pairs[0]);
size_t HttpRequest::_maxUriSize = TMP_HTTP_BUFFER_SIZE;
size_t HttpRequest::_maxVersionSize = TMP_HTTP_BUFFER_SIZE;

std::map<std::string, HttpMethod> HttpRequest::implementedHttpMethod(pairs, pairs + httpMethodCount);

HttpRequest::HttpRequest()
	: HttpMessage(),
	  _method(UNKNOWN),
	  _uri(),
	  _firstLineState(HttpRequest::REQUEST_METHOD) {
	this->_maxMethodSize = this->getMaxMethodSize();
	this->_body = new BodyStringStream();
}

HttpRequest::HttpRequest(const HttpRequest &other)
	: HttpMessage(other),
	  _method(other._method),
	  _uri(other._uri),
	  _firstLineState(other._firstLineState),
	  _maxMethodSize(other._maxMethodSize) {}

// HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
// 	if (this != &other) {
// 		this->HttpMessage::operator=(other);
// 		this->_method = other._method;
// 		this->_uri = other._uri;
// 		this->_firstLineState = other._firstLineState;
// 		this->_maxMethodSize = other._maxMethodSize;
// 	}
// 	return *this;
// }

HttpRequest::~HttpRequest() {}

size_t HttpRequest::getMaxMethodSize() {
	size_t result = 0;

	for (std::map<std::string, HttpMethod>::const_iterator it = this->implementedHttpMethod.begin(); it != this->implementedHttpMethod.end(); ++it) {
		if (it->first.size() > result) result = it->first.size();
	}
	return result;
}

std::string HttpRequest::getAllowHeader() {
	std::string allowHeader;

	for (std::map<std::string, HttpMethod>::const_iterator it = HttpRequest::implementedHttpMethod.begin(); it != HttpRequest::implementedHttpMethod.end(); ++it) {
		if (!allowHeader.empty())
			allowHeader += ", ";
		allowHeader += it->first;
	}
	return allowHeader;
}

HttpMethod HttpRequest::method() const {
	return this->_method;
}

std::string HttpRequest::methodStr() const {
	return httpMethodString(this->_method);
}

const std::string &HttpRequest::uri() const {
	return this->_uri;
}

std::ostream &HttpRequest::printTypeInfo(std::ostream &os) const {
	return os << this->methodStr() << " " << this->_uri << " " << this->versionStr() << '\n';
}
