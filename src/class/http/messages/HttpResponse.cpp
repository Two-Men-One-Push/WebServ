#include "./HttpResponse.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "http/HttpStatus.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/types.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

HttpResponse::HttpResponse(HttpTransaction &connection) : HttpMessage(connection), _status(HttpStatus::NoStatus) {}

HttpResponse::HttpResponse(const HttpResponse &other, HttpTransaction &connection) : HttpMessage(other, connection), _status(other._status) {}

HttpResponse::~HttpResponse() {}

/**
 * This behavior is described by the RFC depending of the version
 */
bool HttpResponse::hasBody() const {
	if (this->_version == HTTP1_0) {
		return false;
	} else {
		/** @see https://datatracker.ietf.org/doc/html/rfc2616#section-4.4 first case talking about response messages (aka HTTP response) */
		if (HttpStatus::isInformational(this->_status) || this->_status == HttpStatus::NoContent || this->_status == HttpStatus::NotModified)
			return false;
		return this->HttpMessage::hasBody();
	}
}

bool HttpResponse::appendMessageTypes(std::istream &input) {
	std::string &buffer = this->_buffer;

	// Extract first line of response
	while (true) {
		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			break;
		}
	}

	// parse message types
	std::string line = buffer.substr(0, buffer.size() - 2);
	buffer = std::string();
	std::cout << "[line] " << line << std::endl;
	return true;
}

void HttpResponse::error(const HttpException &e) {
	std::cout << e.what() << std::endl;

	this->_status = e.status();
	this->_message = e.message();

	if (HttpStatus::isError(e.status())) {
		this->setHeader("Connection", "close");
	}
	if (e.status() == 405) {
		this->setHeader("Allow", HttpRequest::getAllowHeader());
	}
}

bool HttpResponse::send(ClientSocket &clientSocket) {
	(void)clientSocket;
	return true;
}

void HttpResponse::loadTypeUsedHeaders() {
	return;
}

std::ostream &HttpResponse::printTypeInfo(std::ostream &os) const {
	return os << this->_version << " " << this->_status << '\n';
}
