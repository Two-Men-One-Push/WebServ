#include "./HttpTransaction.hpp"
#include "CGI/CGIInterface.hpp"
#include "WebServer/WebServer.hpp"
#include "http/errors/HttpException.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <iostream>

HttpTransaction::HttpTransaction() : _request(), _response(), _last(false) {
	std::cout << "New HTTP transaction created" << std::endl;
}

HttpTransaction::HttpTransaction(const HttpTransaction &other) : _request(other._request), _response(other._response), _last(other._last) {}

HttpTransaction::~HttpTransaction() {}

bool HttpTransaction::recvRequest(std::istream &input, WebServer &server) {
	(void)server;
	try {
		bool result = this->_request.recvFrom(input);
		if (result) {
			this->_response.error(HttpExceptions::NoContentException());
		}
		return result;
	} catch (const HttpException &e) {
		this->error(e);
		return true;
	}
}

bool HttpTransaction::recvResponse(std::istream &input) {
	try {
		return this->_response.recvFrom(input);
	} catch (const HttpException &e) {
		this->error(e);
		return true;
	}
}

bool HttpTransaction::sendRequest(const AFd &output) {
	return this->_request.sendTo(output);
}

bool HttpTransaction::sendRequestBody(const AFd &output) {
	return this->_response.sendBody(output);
}

bool HttpTransaction::sendResponse(const AFd &output) {
	return this->_response.sendTo(output);
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

bool HttpTransaction::last() {
	return this->_last ||
		   (this->_response.headers().has("Connection") &&
			this->_response.headers()["Connection"] == "close");
}
