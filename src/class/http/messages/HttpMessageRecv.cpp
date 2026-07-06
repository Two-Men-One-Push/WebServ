#include "./HttpMessage.hpp"
#include "errors/WebservErrors.hpp"
#include "http/errors/HttpStandardException.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <unistd.h>
#include <utility>

bool HttpMessage::recvFrom(std::istream &input) {
	std::cerr << "receiving " << static_cast<std::stringstream &>(input).str().size() << " bytes" << std::endl;
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
	this->loadConnection();
	this->loadContentLength();
}

bool HttpMessage::hasBody() const {
	return this->_transferEncoding != TE_UNDEFINED || this->_inputWillClose || this->_contentLength != 0;
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
	std::string &buffer = this->_inBuffer;

	std::pair<std::string, std::string> headerField;
	while (true) {
		while (true) {
			if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

			int c = input.get();
			if (c == std::stringstream::traits_type::eof())
				return false;
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
			buffer.clear();
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

bool HttpMessage::recvBody(std::istream &input) {
	if (this->_transferEncoding == TE_UNDEFINED) return this->collectRawBody(input);
	if (this->_transferEncoding == TE_CHUNKED) return this->collectChunkedBody(input);
	throw HttpExceptions::NotImplementedException();
}

void HttpMessage::writeBody(const char *buffer, size_t size) {
	while (size) {
		ssize_t written = this->_body->write(buffer, size);
		if (written < 0) throw WebservErrors::SysError("write", errno);
		if (written == 0)
			throw HttpExceptions::InternalServerErrorException();
		buffer += written;
		size -= written;
	}
}

bool HttpMessage::collectRawBody(std::istream &input) {
	char buffer[READ_SIZE];

	while (true) {
		size_t toRead;
		if (!this->_inputWillClose) {
			toRead = std::min(sizeof(buffer), this->_contentLength - this->_readSize);
			if (toRead == 0)
				break;
		} else {
			toRead = READ_SIZE;
		}
		input.read(buffer, toRead);
		std::streamsize n = input.gcount();
		if (n == 0) {
			break;
		}
		this->writeBody(buffer, n);
		if (!this->_inputWillClose) {
			this->_readSize += n;
		} else {
			this->_contentLength += n;
		}
	}

	if (!this->_inputWillClose) {
		return this->_readSize >= this->_contentLength;
	} else {
		return false;
	}
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
		if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

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
		if (buffer.size() == 0 || !ishexdigit(buffer[0])) throw HttpExceptions::BadRequestException();
		buffer = trim(buffer, " ");
	}
	if (buffer.size() == 0) throw HttpExceptions::BadRequestException();
	try {
		this->_chunkInfo.size = parseHex(buffer);
		this->_chunkInfo.readSize = 0;
	} catch (...) {
		throw HttpExceptions::BadRequestException();
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
		if (n == 0)
			break;
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
	if (buffer != "\r\n") throw HttpExceptions::BadRequestException();
	buffer.clear();
	this->_chunkInfo.state = BodyChunkInfo::CHUNK_SIZE;
	return true;
}

bool HttpMessage::getChunkedTrailer(std::istream &input) {
	std::string &buffer = this->_inBuffer;

	while (true) {
		if (buffer.size() == TMP_HTTP_BUFFER_SIZE) throw HttpExceptions::BadRequestException();

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
