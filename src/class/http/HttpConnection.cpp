#include "./HttpConnection.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/messages/HttpResponse.hpp"

HttpConnection::HttpConnection() : _request(*this), _response(*this), _last(false) {}

HttpConnection::HttpConnection(const HttpConnection &other) : _request(other._request, *this), _response(other._response, *this), _last(other._last) {}

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

void HttpConnection::error(const HttpException &e) {
	this->_response.error(e);
	this->_last = true;
}
