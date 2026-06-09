#include "./HttpTransaction.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <iostream>

HttpTransaction::HttpTransaction() : _request(*this), _response(*this), _last(false) {
	std::cout << "New HTTP transaction created" << std::endl;
}

HttpTransaction::HttpTransaction(const HttpTransaction &other) : _request(other._request, *this), _response(other._response, *this), _last(other._last) {}

HttpTransaction::~HttpTransaction() {}

HttpRequest &HttpTransaction::request() {
	return this->_request;
}

const HttpRequest &HttpTransaction::request() const {
	return this->_request;
}

HttpResponse &HttpTransaction::response() {
	return this->_response;
}

const HttpResponse &HttpTransaction::response() const {
	return this->_response;
}

void HttpTransaction::error(const HttpException &e) {
	this->_response.error(e);
	this->_last = true;
}
