#include "http/errors/HttpStandardException.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <cctype>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

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
	if (!this->extractRequestLine(input))
		return false;

	std::string &buffer = this->_buffer;
	std::string line = buffer.substr(0, buffer.size() - 2);
	buffer.clear();

	// parse request line

	std::vector<std::string> words;
	split(line, " ", words);

	if (words.size() != 3) throw HttpExceptions::BadRequestException();
	this->_method = HttpRequest::parseRequestMethod(words[0]);
	this->_uri = words[1];
	this->_version = HttpMessage::parseHttpVersion(words[2]);

	return true;
}

HttpMethod HttpRequest::parseRequestMethod(const std::string &input) {
	if (!istoken(input)) throw HttpExceptions::BadRequestException();
	if (HttpRequest::implementedHttpMethod.find(input) == HttpRequest::implementedHttpMethod.end()) throw HttpExceptions::NotImplementedException();
	return HttpRequest::implementedHttpMethod[input];
}

bool HttpRequest::extractRequestLine(std::istream &input) {
	std::string &buffer = this->_buffer;

	while (true) {
		if (buffer.size() == HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

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
