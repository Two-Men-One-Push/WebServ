#include "./HttpMessage.hpp"
#include "http/HttpTransaction.hpp"
#include "http/errors/HttpException.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <utility>

bool HttpMessage::append(std::istream &input) {
	try {
		switch (this->_state) {
		case HttpMessage::MESSAGE_TYPES:
			if (!this->appendMessageTypes(input)) return false;
			this->_state = HttpMessage::MESSAGE_HEADERS;
			// fallthrough
		case HttpMessage::MESSAGE_HEADERS:
			if (!this->appendMessageHeaders(input)) return false;
			this->_state = HttpMessage::LOAD_MESSAGE_HEADERS;
			// fallthrough
		case HttpMessage::LOAD_MESSAGE_HEADERS:
			this->loadBaseUsedHeaders();
			this->loadTypeUsedHeaders();
			this->_state = HttpMessage::MESSAGE_BODY;
			// fallthrough
		case HttpMessage::MESSAGE_BODY:
			if (this->hasBody() && !this->collectBody(input)) return false;
			this->_state = HttpMessage::COMPLETED;
			std::cout << *this << std::endl;
			return true;
		case HttpMessage::COMPLETED:
			return true;
		}
	} catch (const HttpException &e) {
		this->_transaction.error(e);
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

void HttpMessage::loadBaseUsedHeaders() {
	this->loadTranferEncoding();
	this->loadContentLenght();
}

bool HttpMessage::hasBody() const {
	return this->_transferEncoding != TE_UNDEFINED || this->_contentLength != 0;
}

bool HttpMessage::completed() const {
	return this->_state == HttpMessage::COMPLETED;
}

/**
 * message-header = field-name ":" [ field-value ]
 *		field-name     = token
 *		field-value    = *( field-content | LWS )
 *		field-content  = <the OCTETs making up the field-value
 *						 and consisting of either *TEXT or combinations
 *						 of token, separators, and quoted-string>
 */

bool HttpMessage::appendMessageHeaders(std::istream &input) {
	if (!this->extractMessageHeaders(input)) {
		return false;
	}

	std::string &buffer = this->_buffer;

	std::pair<std::string, std::string> headerField;
	// parse message headers
	size_t pos = 0;
	while (true) {
		// get next header line
		size_t lineEnd = buffer.find("\r\n", pos);
		std::string line = buffer.substr(pos, lineEnd - pos);
		pos = lineEnd + 2;

		// if empty it's the end of headers
		if (line.empty()) {
			if (!headerField.first.empty()) this->_headers.insert(headerField);
			return true;
		}

		if (line[0] == ' ' || line[0] == '\t') {
			if (headerField.first.empty()) throw HttpExceptions::BadRequestException();
			if (!headerField.second.empty()) headerField.second += " ";
			headerField.second += trim(line);
		} else {
			if (!headerField.first.empty()) {
				this->_headers.insert(headerField);
			}
			size_t headerNamePos = line.find(':');
			if (headerNamePos == line.npos) throw HttpExceptions::BadRequestException();
			headerField.first = line.substr(0, headerNamePos);
			if (!istoken(headerField.first)) throw HttpExceptions::BadRequestException();
			toHeaderCase(headerField.first);
			headerField.second = trim(line.substr(headerNamePos + 1));
		}
	}
}

bool HttpMessage::extractMessageHeaders(std::istream &input) {
	// Extract content until end of headers ("\r\n\r\n")
	std::string &buffer = this->_buffer;

	while (true) {
		if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

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
	if (this->_transferEncoding == TE_UNDEFINED) return this->collectRawBody(input);
	if (this->_transferEncoding == TE_CHUNKED) throw HttpExceptions::NotImplementedException();
	throw HttpExceptions::NotImplementedException();
}

bool HttpMessage::collectRawBody(std::istream &input) {
	char buffer[4096];

	std::streamsize n = input.readsome(buffer, std::min(sizeof(buffer), this->_contentLength - this->_readContentLength));
	this->_body.append(buffer, n);
	this->_readContentLength += n;
	if (this->_readContentLength < this->_contentLength) return false;
	return true;
}
