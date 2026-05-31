#include "./HttpConnection.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/messages/HttpResponse.hpp"

HttpConnection::HttpConnection() : _request(*this), _response(*this), _close(false) {}

HttpConnection::HttpConnection(const HttpConnection &other) : _request(other._request, *this), _response(other._response, *this), _close(other._close) {}

// HttpConnection &HttpConnection::operator=(const HttpConnection &other) {
// 	if (this == &other) return *this;
// 	this->_request =
// 	return this;

// }

HttpConnection::~HttpConnection() {}

HttpRequest &HttpConnection::request() {
	return this->_request;
}

const HttpRequest &HttpConnection::request() const {
	return this->_request;
}

HttpResponse &HttpConnection::response() {
	return this->_response;
}

const HttpResponse &HttpConnection::response() const {
	return this->_response;
}

bool HttpConnection::close() const {
	return this->_close;
}

void HttpConnection::close(bool close) {
	this->_close = close;
}
