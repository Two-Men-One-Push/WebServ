#include "http/messages/response/HttpResponse.hpp"
#include <iostream>
#include <sstream>

bool HttpResponse::recvTypeLine(std::istream &input) {
	std::string &buffer = this->_inBuffer;

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
	std::string versionInput = buffer.substr(0, buffer.size() - 2);
	this->_version = this->parseHttpVersion(versionInput);
	return true;
}

void HttpResponse::loadTypeUsedHeaders() {
	return;
}
