#include "http/errors/HttpStandardException.hpp"
#include "http/messages/HttpMessage.hpp"
#include "./HttpRequest.hpp"
#include "utils/parsing.hpp"
#include <cctype>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>

/**
 * @brief HTTP request-line grammar
 *
 * ─────────────────────────────────
 *
 * `Request-Line = Method SP Request-URI SP HTTP-Version CRLF`
 *
 * `Method = "OPTIONS" | "GET" | "HEAD" | "POST" | "PUT" | "DELETE" | "TRACE" | "CONNECT" | extension-method`
 *
 * `extension-method = token`
 *
 * @note
 * Extension methods are not implemented.
 * They are only accepted so the parser can
 * return a proper `400 Bad Request`.
 *
 * `HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT`
 *
 * `Request-URI = "*" | absoluteURI | abs_path | authority`
 *
 * ─────────────────────────────────
 *
 * @see RFC 2616 Section 5.1 https://datatracker.ietf.org/doc/html/rfc2616#section-5.1
 */
bool HttpRequest::appendMessageTypes(std::istream &input) {
	switch (this->_firstLineState) {
	case HttpRequest::REQUEST_METHOD:
		if (!this->parseRequestMethod(input)) return false;
		this->_firstLineState = HttpRequest::REQUEST_URI;
		// fallthrough
	case HttpRequest::REQUEST_URI:
		if (!this->parseRequestUri(input)) return false;
		this->_firstLineState = HttpRequest::REQUEST_VERSION;
		// fallthrough
	case HttpRequest::REQUEST_VERSION:
		if (!this->parseRequestVersion(input)) return false;
	}
	return true;
}

bool HttpRequest::parseRequestMethod(std::istream &input) {
	std::string &buffer = this->_buffer;
	std::string part;

	std::getline(input, part, ' ');
	buffer += part;

	if (!istoken(buffer)) throw HttpExceptions::BadRequestException();
	if (buffer.size() > this->_maxMethodSize) throw HttpExceptions::NotImplementedException();
	if (input.eof()) return false;
	if (HttpRequest::implementedHttpMethod.find(buffer) == HttpRequest::implementedHttpMethod.end()) throw HttpExceptions::NotImplementedException();
	this->_method = HttpRequest::implementedHttpMethod[buffer];
	buffer.clear();
	return true;
}

bool HttpRequest::parseRequestUri(std::istream &input) {
	std::string &buffer = this->_buffer;
	std::string part;

	std::getline(input, part, ' ');
	buffer += part;

	if (buffer.size() > this->_maxUriSize) throw HttpExceptions::URITooLongException();
	if (input.eof()) return false;

	// !:! other checks required here

	this->_uri = buffer;
	buffer.clear();
	return true;
}

bool HttpRequest::parseRequestVersion(std::istream &input) {
	std::string &buffer = this->_buffer;
	std::string part;

	std::getline(input, part, '\n');

	buffer += part;

	if (buffer.size() > this->_maxVersionSize) throw HttpExceptions::BadRequestException();
	if (input.eof()) return false;

	if (buffer[buffer.size() - 1] != '\r') throw HttpExceptions::BadRequestException();
	buffer.resize(buffer.size() - 1);
	this->_version = HttpMessage::parseHttpVersion(buffer);
	buffer.clear();
	return true;
}

bool HttpRequest::extractRequestLine(std::istream &input) {
	std::string &buffer = this->_buffer;

	while (true) {
		if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

		int c = input.get();
		if (c == std::stringstream::traits_type::eof()) return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			if (buffer.size() == 2) {
				buffer.clear();
				continue;
			}
			return true;
		}
	}
}

void HttpRequest::loadTypeUsedHeaders() {
	return;
}
