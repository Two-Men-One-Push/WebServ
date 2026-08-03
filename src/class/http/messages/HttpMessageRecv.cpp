#include "./HttpMessage.hpp"
#include "http/HttpStatus.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/types.hpp"
#include "model/Location/Location.hpp"
#include "utils/parsing.hpp"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <sstream>
#include <unistd.h>
#include <utility>

bool HttpMessage::recvFrom(std::istream &input, const Location &nearestConfig) {
	try {
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
			this->loadCommonHeaders();
			this->loadTypeHeaders();
			this->checkBodyType();
			this->_inState = HttpMessage::RECV_WAITING_ROUTING;
			return false;
		case HttpMessage::RECV_WAITING_ROUTING:
			return false;
		case HttpMessage::RECV_MESSAGE_BODY:
			if (this->hasBody() && !this->recvBody(input, nearestConfig)) return false;
			this->_inState = HttpMessage::RECV_COMPLETED;
			// std::cerr << *this << std::endl;
			// fallthrough
		case HttpMessage::RECV_COMPLETED:
			return true;
		}
	} catch (...) {
		this->_inState = RECV_COMPLETED;
		throw;
	}
	return this->_inState == HttpMessage::RECV_COMPLETED;
}

HttpVersion HttpMessage::parseHttpVersion(const std::string &input) {
	if (input == "HTTP/1.0") return HTTP1_0;
	if (input == "HTTP/1.1") return HTTP1_1;

	if (input.compare(0, 5, "HTTP/")) throw HttpMessage::Exception();
	bool point = false;
	for (std::string::const_iterator it = input.begin() + 5; it != input.end(); ++it) {
		if (*it == '.') {
			if (point) throw HttpMessage::Exception();
			point = true;
		} else if (!std::isdigit(*it)) {
			throw HttpMessage::Exception();
		}
	}
	throw HttpMessage::Exception().requestStatus(HttpStatus::HTTPVersionNotSupported);
}

void HttpMessage::loadCommonHeaders() {
	if (this->_headers.has("Transfer-Encoding") && this->_headers.has("Content-Length")) throw HttpMessage::Exception();

	this->loadTranferEncoding();
	this->loadContentLength();
	this->loadConnection();
	this->loadContentType();
}

void HttpMessage::checkBodyType() {
	if (this->_version == HTTP1_0) {
		if (this->_contentLength > 0) {
			this->_bodyType = BT_CONTENT_LENGTH;
		} else {
			this->_bodyType = BT_EOF;
		}
	} else {
		if (this->_transferEncoding == TE_CHUNKED) {
			this->_bodyType = BT_CHUNKED;
		} else if (this->_contentLength > 0) {
			this->_bodyType = BT_CONTENT_LENGTH;
		} else {
			this->_bodyType = BT_NONE;
		}
	}
}

bool HttpMessage::isWaitingRouting() const {
	return this->_inState == HttpMessage::RECV_WAITING_ROUTING;
}

void HttpMessage::completeRouting() {
	this->_inState = RECV_MESSAGE_BODY;
}

bool HttpMessage::inCompleted() const {
	return this->_inState == HttpMessage::RECV_COMPLETED;
}

bool HttpMessage::hasBody() const {
	return this->_bodyType != BT_NONE;
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
	std::string &buffer = this->_inBuffer;

	std::pair<std::string, std::string> &headerField = this->_bufferedHeaderField;
	while (true) {
		while (true) {
			if (buffer.size() == HTTP_BUFFER_SIZE) throw HttpMessage::Exception();

			int c = input.get();
			if (c == std::stringstream::traits_type::eof()) {
				return false;
			}
			buffer += static_cast<char>(c);
			if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
				buffer.resize(buffer.size() - 2);
				break;
			}
		}
		// get next header line
		std::string line = buffer;
		buffer.clear();
		// if empty it's the end of headers
		if (line.empty()) {
			if (!headerField.first.empty()) this->_headers.insert(headerField);
			return true;
		}

		if (line[0] == ' ' || line[0] == '\t') {
			if (headerField.first.empty()) throw HttpMessage::Exception();
			if (!headerField.second.empty()) headerField.second += " ";
			headerField.second += trim(line);
		} else {
			if (!headerField.first.empty()) {
				this->_headers.insert(headerField);
			}
			size_t headerNamePos = line.find(':');
			if (headerNamePos == line.npos) throw HttpMessage::Exception();
			headerField.first = line.substr(0, headerNamePos);
			if (!istoken(headerField.first)) throw HttpMessage::Exception();
			toHeaderCase(headerField.first);
			headerField.second = trim(line.substr(headerNamePos + 1));
			if (!isheadervalue(headerField.second)) throw HttpMessage::Exception();
		}
	}
}

bool HttpMessage::recvBody(std::istream &input, const Location &nearestConfig) {
	if (this->_contentLength > nearestConfig.clientMaxBodySize()) throw HttpMessage::Exception().requestStatus(HttpStatus::ContentTooLarge).responseStatus(HttpStatus::BadGateway);
	bool result;
	if (this->_bodyType == BT_CHUNKED) result = this->collectChunkedBody(input);
	else if (this->_bodyType == BT_CONTENT_LENGTH) result = this->collectRawBody(input);
	else if (this->_bodyType == BT_EOF) result = this->collectRawBodyToEOF(input);
	else throw HttpMessage::Exception().requestStatus(HttpStatus::NotImplemented);
	if (this->_contentLength > nearestConfig.clientMaxBodySize()) throw HttpMessage::Exception().requestStatus(HttpStatus::ContentTooLarge).responseStatus(HttpStatus::BadGateway);
	return result;
}

void HttpMessage::writeBody(const char *buffer, size_t size) {
	while (size) {
		ssize_t written = this->_body->write(buffer, size);
		if (written < 0) throw HttpErrors::InternalServerErrorException();
		buffer += written;
		size -= written;
	}
}

bool HttpMessage::collectRawBody(std::istream &input) {
	char buffer[READ_SIZE];

	while (true) {
		size_t toRead = std::min(sizeof(buffer), this->_contentLength - this->_readSize);
		if (toRead == 0)
			break;

		input.read(buffer, toRead);

		std::streamsize n = input.gcount();
		if (n == 0) break;

		this->writeBody(buffer, n);
		this->_readSize += n;
	}

	return this->_readSize >= this->_contentLength;
}

bool HttpMessage::collectRawBodyToEOF(std::istream &input) {
	char buffer[READ_SIZE];

	while (true) {
		input.read(buffer, READ_SIZE);

		std::streamsize n = input.gcount();
		if (n == 0) break;

		this->writeBody(buffer, n);
		this->_contentLength += n;
	}

	return false;
}

bool HttpMessage::collectChunkedBody(std::istream &input) {
	while (input.peek() != std::stringstream::traits_type::eof()) {
		switch (this->_chunkInfo.state) {
		case BodyChunkInfo::CHUNK_SIZE:
			if (!this->getChunkSize(input)) return false;
			break;
		case BodyChunkInfo::CHUNK_CONTENT:
			if (!this->getChunkContent(input)) return false;
			break;
		case BodyChunkInfo::CHUNK_CRLF:
			if (!this->getChunkCrlf(input)) return false;
			break;
		case BodyChunkInfo::CHUNK_TRAILER:
			if (!this->getChunkedTrailer(input)) return false;
			break;
		case BodyChunkInfo::CHUNK_COMPLETED:
			return true;
		}
	}
	return this->_chunkInfo.state == BodyChunkInfo::CHUNK_COMPLETED;
}

bool HttpMessage::getChunkSize(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	while (true) {
		if (buffer.size() == HTTP_BUFFER_SIZE) throw HttpMessage::Exception();

		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			buffer.resize(buffer.size() - 2);
			break;
		}
	}

	size_t pos = buffer.find(';');
	if (pos != buffer.npos) {
		buffer.resize(pos);
		if (buffer.size() == 0 || !ishexdigit(buffer[0])) throw HttpMessage::Exception();
		buffer = trim(buffer, " ");
	}
	if (buffer.size() == 0) throw HttpMessage::Exception();
	try {
		this->_chunkInfo.size = parseHex(buffer);
		this->_chunkInfo.readSize = 0;
	} catch (...) {
		throw HttpMessage::Exception();
	}
	buffer.clear();
	if (this->_chunkInfo.size == 0) {
		this->_chunkInfo.state = BodyChunkInfo::CHUNK_TRAILER;
	} else {
		this->_chunkInfo.state = BodyChunkInfo::CHUNK_CONTENT;
	}
	return true;
}

bool HttpMessage::getChunkContent(std::istream &input) {
	char buffer[READ_SIZE];
	while (true) {
		size_t toRead = std::min(this->_chunkInfo.size - this->_chunkInfo.readSize, (size_t)READ_SIZE);
		if (toRead == 0)
			break;
		input.read(buffer, toRead);

		std::streamsize n = input.gcount();
		if (n == 0) break;

		this->writeBody(buffer, n);
		this->_chunkInfo.readSize += n;
	}

	if (this->_chunkInfo.readSize < this->_chunkInfo.size) return false;
	this->_chunkInfo.state = BodyChunkInfo::CHUNK_CRLF;
	return true;
}

bool HttpMessage::getChunkCrlf(std::istream &input) {
	std::string &buffer = this->_inBuffer;
	while (buffer.size() < 2) {
		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
	}
	if (buffer != "\r\n") throw HttpMessage::Exception();
	buffer.clear();
	this->_chunkInfo.state = BodyChunkInfo::CHUNK_SIZE;
	return true;
}

bool HttpMessage::getChunkedTrailer(std::istream &input) {
	std::string &buffer = this->_inBuffer;

	while (true) {
		if (buffer.size() == HTTP_BUFFER_SIZE) throw HttpMessage::Exception();

		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 2 && buffer.compare(buffer.size() - 2, 2, "\r\n") == 0) {
			if (buffer.size() > 2) {
				buffer.clear();
			} else {
				buffer.clear();
				this->_chunkInfo.state = BodyChunkInfo::CHUNK_COMPLETED;
				return true;
			}
		}
	}
}
