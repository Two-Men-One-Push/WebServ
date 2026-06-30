#include "./HttpMessage.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <utility>

bool HttpMessage::recvFrom(std::istream &input) {
	switch (this->_inState) {
	case HttpMessage::RECV_MESSAGE_TYPES:
		if (!this->recvTypeLine(input)) return false;
		this->_inState = HttpMessage::RECV_MESSAGE_HEADERS;
		// fallthrough
	case HttpMessage::RECV_MESSAGE_HEADERS:
		if (!this->recvMessageHeaders(input)) return false;
		this->_inState = HttpMessage::RECV_LOAD_MESSAGE_HEADERS;
		// fallthrough
	case HttpMessage::RECV_LOAD_MESSAGE_HEADERS:
		this->loadBaseUsedHeaders();
		this->loadTypeUsedHeaders();
		this->_inState = HttpMessage::RECV_MESSAGE_BODY;
		// fallthrough
	case HttpMessage::RECV_MESSAGE_BODY:
		if (this->hasBody() && !this->recvBody(input)) return false;
		this->_inState = HttpMessage::RECV_COMPLETED;
		std::cerr << *this << std::endl;
		// fallthrough
	case HttpMessage::RECV_COMPLETED:
		return true;
	}
	return this->_inState == HttpMessage::RECV_COMPLETED;
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
	if (this->_headers.has("Transfer-Encoding") && this->_headers.has("Content-Length")) {
		throw HttpExceptions::BadRequestException();
	}
	this->loadTranferEncoding();
	this->loadContentLength();
}

bool HttpMessage::hasBody() const {
	return this->_transferEncoding != TE_UNDEFINED || this->_contentLength != 0;
}

bool HttpMessage::inCompleted() const {
	return this->_inState == HttpMessage::RECV_COMPLETED;
}

/**
 * message-header = field-name ":" [ field-value ]
 *		field-name     = token
 *		field-value    = *( field-content | LWS )
 *		field-content  = <the OCTETs making up the field-value
 *						 and consisting of either *TEXT or combinations
 *						 of token, separators, and quoted-string>
 */

bool HttpMessage::recvMessageHeaders(std::istream &input) {
	if (!this->extractMessageHeaders(input)) {
		return false;
	}

	std::string &buffer = this->_inBuffer;

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
	std::string &buffer = this->_inBuffer;

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

bool HttpMessage::recvBody(std::istream &input) {
	if (this->_transferEncoding == TE_UNDEFINED) return this->collectRawBody(input);
	if (this->_transferEncoding == TE_CHUNKED) throw HttpExceptions::NotImplementedException();
	throw HttpExceptions::NotImplementedException();
}

bool HttpMessage::collectRawBody(std::istream &input) {
	char buffer[READ_SIZE];
	std::streamsize n;

	while ((n = input.readsome(buffer, std::min(sizeof(buffer), std::min(this->_contentLength - this->_readSize, (size_t)READ_SIZE)))) > 0) {
		this->_body->write(buffer, n);
		this->_readSize += n;
	}

	if (this->_readSize < this->_contentLength) return false;
	return true;
}

bool HttpMessage::collectChunkedBody(std::istream &input) {
	std::string &buffer = this->_inBuffer;

	while (input.peek() != std::stringstream::traits_type::eof()) {
		if (this->chunkInfo.size == this->chunkInfo.readSize) {
			while (true) {
				if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

				int c = input.get();
				if (c == std::stringstream::traits_type::eof())
					return false;
				buffer += static_cast<char>(c);
				if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
					break;
				}
			}
		} else {

		}
	}

	return false;
}
