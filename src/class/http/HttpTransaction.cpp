#include "./HttpTransaction.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <iostream>

HttpTransaction::HttpTransaction() : _request(), _response(), _last(false) {
	std::cout << "New HTTP transaction created" << std::endl;
}

HttpTransaction::HttpTransaction(const HttpTransaction &other) : _request(other._request), _response(other._response), _last(other._last) {}

HttpTransaction::~HttpTransaction() {}

bool HttpTransaction::appendToRequest(std::istream &input) {
	try {
		return this->_request.recvFrom(input);
	} catch (const HttpException &e) {
		this->error(e);
		return true;
	}
}

bool HttpTransaction::appendToResponse(std::istream &input) {
	try {
		return this->_request.recvFrom(input);
	} catch (const HttpException &e) {
		this->error(e);
		return true;
	}
}

const HttpRequest &HttpTransaction::request() const {
	return this->_request;
}

const HttpResponse &HttpTransaction::response() const {
	return this->_response;
}

void HttpTransaction::error(const HttpException &e) {
	this->_response.error(e);
	this->_last = true;
}
