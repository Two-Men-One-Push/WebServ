#include "http/HttpStatus.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <iostream>

void HttpResponse::error(const HttpException &e) {
	this->_status = e.status();
	this->_message = e.message();

	std::cerr << e.what() << std::endl;
	if (HttpStatus::isServerError(e.status())) {
		// std::cerr << e.what() << std::endl;
	}

	if (HttpStatus::isError(e.status())) {
		this->setHeader("Connection", "close");
	}

	if (e.status() == 405) {
		this->setHeader("Allow", HttpRequest::getAllowHeader());
	}

	// More specific error handling

	this->end();
}
