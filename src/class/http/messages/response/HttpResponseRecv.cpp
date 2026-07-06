#include "http/HttpStatus.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <sstream>

// bool HttpResponse::recvTypeLine(std::istream &input) {
// 	std::string &buffer = this->_inBuffer;

// 	// Extract first line of response
// 	while (true) {
// 		int c = input.get();
// 		if (c == std::stringstream::traits_type::eof())
// 			return false;
// 		buffer += static_cast<char>(c);
// 		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
// 			break;
// 		}
// 	}

// 	// parse message types
// 	std::string versionInput = buffer.substr(0, buffer.size() - 2);
// 	this->_version = this->parseHttpVersion(versionInput);
// 	return true;
// }

bool HttpResponse::recvTypeLine(std::istream &input) {
	switch (this->_firstLineState) {
	case HttpResponse::RESPONSE_VERSION:
		if (!this->parseResponseVersion(input)) return false;
		this->_firstLineState = HttpResponse::RESPONSE_STATUS;
		// fallthrough
	case HttpResponse::RESPONSE_STATUS:
		if (!this->parseResponseStatus(input)) return false;
		this->_firstLineState = HttpResponse::RESPONSE_MESSAGE;
		// fallthrough
	case HttpResponse::RESPONSE_MESSAGE:
		if (!this->parseResponseMessage(input)) return false;
	}
	return true;
}

bool HttpResponse::parseResponseVersion(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	std::string part;

	while (true) {
		if (buffer.size() == this->_maxVersionSize) throw HttpExceptions::InternalServerErrorException();

		int c = input.get();
		if (c == ' ')
			break;
		if (c == std::stringstream::traits_type::eof())
			return false;
		if (!istokenc(c)) throw HttpExceptions::InternalServerErrorException();
		buffer += static_cast<char>(c);
	}

	this->_version = HttpMessage::parseHttpVersion(buffer);
	buffer.clear();
	return true;
}

bool HttpResponse::parseResponseStatus(std::istream &input) {
	std::string &buffer = this->_inBuffer;

	while (true) {
		/** + 1 for space char*/
		if (buffer.size() == this->_maxStatusSize + 1) throw HttpExceptions::InternalServerErrorException();

		int c = input.get();
		if (c == ' ')
			break;
		if (c == std::stringstream::traits_type::eof())
			return false;
		if (!istokenc(c)) throw HttpExceptions::InternalServerErrorException();
		buffer += static_cast<char>(c);
	}

	try {
		this->_status = HttpStatus::fromInt(parseULong(buffer));
	} catch (...) {
		buffer.clear();
		throw HttpExceptions::InternalServerErrorException();
	}
	buffer.clear();
	return true;
}

bool HttpResponse::parseResponseMessage(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	std::string part;

	buffer += part;

	while (true) {
		if (buffer.size() == this->_maxMessageSize) throw HttpExceptions::URITooLongException();

		int c = input.get();
		if (c == ' ')
			break;
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			buffer.resize(buffer.length() - 2);
		}
	}

	this->_message = buffer;

	// !:! other checks required here

	buffer.clear();
	return true;
}

void HttpResponse::loadTypeUsedHeaders() {
	HeaderMap &headers = this->_headers;
	if (this->_cgiInterface != NULL) {
		if (headers.has("Status")) {
			this->loadCGIStatus(headers.at("Status"));
			headers.erase("Status");
		}
	}
}

void HttpResponse::loadCGIStatus(const std::string &statusString) {
	size_t spacePos = statusString.find(' ');
	if (spacePos == statusString.npos) {
		throw HttpExceptions::InternalServerErrorException();
	}
	try {
		this->_status = HttpStatus::fromInt(parseULong(statusString.substr(0, spacePos)));
	} catch (...) {
		throw HttpExceptions::InternalServerErrorException();
	}
	this->_message = statusString.substr(spacePos + 1);
}

void HttpResponse::closeInput() {
	if (this->_inState == RECV_MESSAGE_BODY) {
		this->_inState = RECV_COMPLETED;
	} else {
		throw HttpExceptions::InternalServerErrorException();
	}
}
