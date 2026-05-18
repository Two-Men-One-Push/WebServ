#include "./HttpRequest.hpp"
#include "http/HttpMessage.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

HttpRequest::HttpRequest() : HttpMessage(),
							 _method(),
							 _path() {}

HttpRequest::HttpRequest(const HttpRequest &other) : HttpMessage(other),
													 _method(other._method),
													 _path(other._path) {}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
	if (this != &other) {
		this->HttpMessage::operator=(other);
		this->_method = other._method;
		this->_path = other._path;
	}
	return *this;
}

HttpRequest::~HttpRequest() {}

bool HttpRequest::appendMessageTypes(std::istream &input) {
	std::string &buffer = this->_buffer;

	// Extract first line of request (types line)
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
