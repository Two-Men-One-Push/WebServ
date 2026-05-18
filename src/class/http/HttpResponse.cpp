#include "./HttpResponse.hpp"
#include "http/HttpMessage.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

HttpResponse::HttpResponse() : HttpMessage(),
							   _status() {}

HttpResponse::HttpResponse(const HttpResponse &other) : HttpMessage(other),
														_status(other._status) {}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {
	if (this != &other) {
		this->HttpMessage::operator=(other);
		this->_status = other._status;
		this->_status = other._status;
	}
	return *this;
}

HttpResponse::~HttpResponse() {}

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
