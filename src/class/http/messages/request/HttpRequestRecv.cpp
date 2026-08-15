#include "./HttpRequest.hpp"
#include "URL/URL.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
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
bool HttpRequest::recvTypeLine(std::istream &input) {
	switch (this->_firstLineState) {
	case HttpRequest::REQUEST_METHOD:
		if (!this->parseRequestMethod(input)) return false;
		this->_firstLineState = HttpRequest::REQUEST_URI;
		// fallthrough
	case HttpRequest::REQUEST_URI:
		if (!this->parseRequestUri(input)) return false;
		if (this->_firstLineState == HttpRequest::REQUEST_LINE_DONE) return true;
		this->_firstLineState = HttpRequest::REQUEST_VERSION;
		// fallthrough
	case HttpRequest::REQUEST_VERSION:
		if (!this->parseRequestVersion(input)) return false;
		this->_firstLineState = HttpRequest::REQUEST_LINE_DONE;
		// fallthrough
	case HttpRequest::REQUEST_LINE_DONE:
		return true;
	}
	return true;
}

bool HttpRequest::parseRequestMethod(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	std::string part;

	while (true) {
		if (buffer.size() == this->_maxMethodSize + 1) throw HttpErrors::NotImplementedException();

		int c = input.get();
		if (c == ' ')
			break;
		if (c == std::stringstream::traits_type::eof())
			return false;
		if (!istokenc(c) && c != '\r' && c != '\n') throw HttpErrors::BadRequestException();
		buffer += static_cast<char>(c);
		if (buffer == "\r\n") buffer.clear();
	}

	if (buffer.find_first_of("\r\n") != std::string::npos) throw HttpErrors::BadRequestException();

	if (HttpRequest::implementedHttpMethod.find(buffer) == HttpRequest::implementedHttpMethod.end()) throw HttpErrors::NotImplementedException();
	this->_method = HttpRequest::implementedHttpMethod[buffer];
	buffer.clear();
	return true;
}

bool HttpRequest::parseRequestUri(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	std::string part;

	buffer += part;

	while (true) {
		if (buffer.size() == this->_maxUriSize) throw HttpErrors::URITooLongException();

		int c = input.get();
		if (c == ' ')
			break;
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			this->_uri = URL("/");
			this->_version = HttpMessage::parseHttpVersion(buffer.substr(0, buffer.size() - 2));
			if (this->_version == HTTP1_0) this->_keepAlive = false;
			buffer.clear();
			this->_firstLineState = HttpRequest::REQUEST_LINE_DONE;
			return true;
		}
	}

	this->_uri = URL(buffer);
	if (this->_uri.format() == URL_ERROR) throw HttpErrors::BadRequestException();
	buffer.clear();
	return true;
}

bool HttpRequest::parseRequestVersion(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	std::string part;

	while (true) {
		if (buffer.size() == this->_maxVersionSize) throw HttpErrors::BadRequestException();
		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			buffer.resize(buffer.size() - 2);
			break;
		}
	}

	this->_version = HttpMessage::parseHttpVersion(buffer);
	if (this->_version == HTTP1_0) this->_keepAlive = false;
	buffer.clear();
	return true;
}

void HttpRequest::insertHeaderField(const std::pair<std::string, std::string> &headerField) {
	HeaderMap &headers = this->_headers;

	if (headers.has(headerField.first)) {
		if (headerField.first == "Host" || headerField.first == "Content-Length") throw HttpErrors::BadRequestException();
		if (!headerField.second.empty()) {
			if (headerField.first == "Cookie") {
				headers[headerField.first] += ";";
			} else {
				headers[headerField.first] += ", ";
			}
			headers[headerField.first] += headerField.second;
		} else if (headers[headerField.first].empty()) {
			headers[headerField.first] = headerField.second;
		}
	} else {
		headers.insert(headerField);
	}
}

void HttpRequest::loadTypeHeaders() {
	this->loadHost();
	return;
}

void HttpRequest::checkBodyType() {
	if (this->_version == HTTP1_0) {
		if (this->_contentLength > 0) {
			this->_bodyType = BT_CONTENT_LENGTH;
		} else {
			this->_bodyType = BT_NONE;
		}
	} else {
		this->HttpMessage::checkBodyType();
	}
}

void HttpRequest::closeInput() {
	throw HttpErrors::BadRequestException();
}
