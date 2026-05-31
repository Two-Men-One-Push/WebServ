#include "./HttpMessage.hpp"
#include "http/HttpConnection.hpp"
#include "http/messages/HttpResponse.hpp"
#include "http/types.hpp"

HttpMessage::HttpMessage(HttpConnection &connection)
	: _state(HttpMessage::MESSAGE_TYPES),
	  _version(),
	  _headers(),
	  _buffer(),
	  _connection(connection) {
	this->_buffer.reserve(HTTP_BUFFER_SIZE);
}

HttpMessage::HttpMessage(const HttpMessage &other, HttpConnection &connection)
	: _state(other._state),
	  _version(other._version),
	  _headers(other._headers),
	  _buffer(other._buffer),
	  _connection(connection) {}

HttpMessage::~HttpMessage() {}

HttpVersion HttpMessage::version() const {
	return this->_version;
}

std::string HttpMessage::versionStr() const {
	return httpVersionString(this->_version);
}

void HttpMessage::setHeader(const std::string &fieldName, const std::string &fieldValue) {
	this->_headers[fieldName] = fieldValue;
}

const Headers &HttpMessage::headers() const {
	return this->_headers;
}

Headers &HttpMessage::headers() {
	return this->_headers;
}
