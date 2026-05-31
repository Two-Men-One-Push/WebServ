#include "./HttpMessage.hpp"
#include "http/HttpConnection.hpp"
#include "http/errors/HttpException.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/types.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <utility>

bool HttpMessage::append(std::istream &input) {
	try {
		switch (this->_state) {
		case HttpMessage::MESSAGE_TYPES:
			if (!this->appendMessageTypes(input))
				return false;
			this->_state = HttpMessage::MESSAGE_HEADERS;
			std::cout << "[type_line] "
					  << dynamic_cast<HttpRequest *>(this)->methodStr() << " "
					  << dynamic_cast<HttpRequest *>(this)->uri() << " "
					  << dynamic_cast<HttpRequest *>(this)->versionStr()
					  << std::endl;

		case HttpMessage::MESSAGE_HEADERS:
			if (!this->appendMessageHeaders(input))
				return false;
			this->_state = HttpMessage::MESSAGE_BODY;

		case HttpMessage::MESSAGE_BODY:
			if (this->hasBody())
				if (!this->collectBody(input))
					return false;
			this->_state = HttpMessage::COMPLETED;
			return true;

		case HttpMessage::COMPLETED:
			return true;
		}
	} catch (const HttpException &e) {
		this->_connection.response().error(e);
		this->_connection.close(true);
		return true;
	}
}

HttpVersion HttpMessage::parseHttpVersion(const std::string &input) {
	if (input == "HTTP/1.0") return HTTP1_0;
	if (input == "HTTP/1.1") return HTTP1_1;

	if (input.compare(0, 5, "HTTP/")) throw HttpExceptions::BadRequestException();
	bool point = false;
	for (std::string::const_iterator it = input.begin() + 5; it != input.end(); ++it) {
		if (*it == '.') {
			if (point) throw HttpExceptions::BadRequestException();
			point = true;
		} else if (!std::isdigit(*it)) {
			throw HttpExceptions::BadRequestException();
		}
	}
	throw HttpExceptions::HTTPVersionNotSupportedException();
}

bool HttpMessage::hasBody() const {
	return false;
}

bool HttpMessage::completed() const {
	return this->_state == HttpMessage::COMPLETED;
}

/**
 * message-header = field-name ":" [ field-value ]
		field-name     = token
		field-value    = *( field-content | LWS )
		field-content  = <the OCTETs making up the field-value
						 and consisting of either *TEXT or combinations
						 of token, separators, and quoted-string>
 */

bool HttpMessage::appendMessageHeaders(std::istream &input) {
	if (!this->extractMessageHeaders(input)) return false;

	std::string &buffer = this->_buffer;

	std::pair<std::string, std::string> headerField;
	// parse message headers
	while (true) {
		size_t pos = buffer.find("\r\n");
		std::string line = buffer.substr(0, pos);
		buffer = buffer.substr(pos + 2);
		std::cout << "[line] " << line << std::endl;
		if (line.empty())
			return true;
	}
}

bool HttpMessage::extractMessageHeaders(std::istream &input) {
	// Extract content until end of headers ("\r\n\r\n")
	std::string &buffer = this->_buffer;

	while (true) {
		if (buffer.size() == HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 4 && buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0) {
			return true;
		}
	}
}

bool HttpMessage::collectBody(std::istream &input) {
	(void)input;
	return true;
}
