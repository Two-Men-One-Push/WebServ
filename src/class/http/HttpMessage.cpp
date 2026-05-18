#include "http/HttpMessage.hpp"
#include <iostream>
#include <sstream>

HttpMessage::HttpMessage() : _version(),
							 _headers(),
							 _state(HttpMessage::MESSAGE_TYPES),
							 _buffer() {}

HttpMessage::HttpMessage(const HttpMessage &other) : _version(other._version),
													 _headers(other._headers),
													 _state(other._state),
													 _buffer(other._buffer) {}

HttpMessage &HttpMessage::operator=(const HttpMessage &other) {
	if (this != &other) {
		this->_version = other._version;
		this->_headers = other._headers;
		this->_state = other._state;
		this->_buffer = other._buffer;
	}
	return *this;
}

bool HttpMessage::completed() const {
	return this->_state == HttpMessage::COMPLETED;
}

bool HttpMessage::append(std::istream &input) {
	switch (this->_state) {
	case HttpMessage::MESSAGE_TYPES:
		if (!this->appendMessageTypes(input))
			return false;
		this->_state = HttpMessage::MESSAGE_HEADERS;
	case HttpMessage::MESSAGE_HEADERS:
		if (!this->appendMessageHeaders(input))
			return false;
		this->_state = HttpMessage::MESSAGE_BODY;
	case HttpMessage::MESSAGE_BODY:
		if (this->hasBody())
			if (!this->appendMessageTypes(input))
				return false;
		this->_state = HttpMessage::COMPLETED;
		return true;
	case HttpMessage::COMPLETED:
		return true;
	}
}

bool HttpMessage::appendMessageHeaders(std::istream &input) {
	std::string &buffer = this->_buffer;

	// Extract content until end of headers ("\r\n\r\n")
	while (true) {
		int c = input.get();
		if (c == std::stringstream::traits_type::eof())
			return false;
		buffer += static_cast<char>(c);
		if (buffer.size() >= 4 && buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0) {
			break;
		}
	}

	// parse message headers
	while (true) {
		size_t pos = buffer.find("\r\n");
		std::string line = buffer.substr(0, pos);
		std::cout << "[line] " << line << std::endl;
		if (line.empty())
			return true;
		buffer = buffer.substr(pos + 2);
	}
}
