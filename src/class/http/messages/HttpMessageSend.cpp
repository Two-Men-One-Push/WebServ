#include "errors/WebservErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <istream>
#include <sstream>
#include <stdexcept>

bool HttpMessage::sendTo(const AFd &output) {
	switch (this->_outState) {
	case SEND_HEAD:
		if (!this->sendHead(output)) return false;
		this->_outState = SEND_BODY;
		return false;
		// fallthrough
	case SEND_BODY:
		if (!this->hasBody()) {
			this->_outState = SEND_COMPLETED;
			return true;
		}
		return this->sendBody(output);
		if (this->_sentSize == this->_contentLength) {
			if (!this->_bodyEmpty) {
			}
			delete this->_body;
			this->_outState = SEND_COMPLETED;
		} else if (this->_bodyEmpty) {
		}
		// fallthrough
	case SEND_COMPLETED:
		return true;
	}
	return this->_outState == SEND_COMPLETED;
}

bool HttpMessage::sendHead(const AFd &output) {
	std::string &buffer = this->_outBuffer;

	if (buffer.empty()) this->formatHead();
	ssize_t sent = output.write(buffer.data(), std::min(buffer.size(), (size_t)WRITE_SIZE));
	buffer.erase(0, sent);
	return buffer.empty();
}

bool HttpMessage::sendBody(const AFd &output) {
	std::string &buffer = this->_outBuffer;
	std::iostream &body = *this->_body;

	size_t maxWrite = std::min(_contentLength, (size_t)WRITE_SIZE);

	if (buffer.size() < maxWrite && !this->_bodyEmpty) {
		size_t oldSize = buffer.size();
		size_t missingSize = maxWrite - oldSize;

		buffer.resize(maxWrite);
		std::cerr << &body << std::endl;
		body.read(&buffer[0] + oldSize, missingSize);

		std::streamsize readSize = body.gcount();
		buffer.resize(oldSize + (size_t)readSize);

		if (this->_body->eof()) {
			this->_bodyEmpty = true;
		} else if (this->_body->fail()) {
			throw WebservErrors::SysError("read", errno);
		}
	}

	ssize_t sent = output.write(buffer.data(), buffer.size());
	if (sent < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		throw WebservErrors::SysError("write", errno);
	}
	this->_sentSize += sent;
	buffer.erase(0, sent);

	if (this->_sentSize == this->_contentLength) {
		return true;
	} else if (buffer.empty() && this->_bodyEmpty) {
		throw std::runtime_error("Not enough body to send");
	} else {
		return false;
	}
}

void HttpMessage::formatHead() {
	std::stringstream ss;

	this->formatTypeLine();

	this->_outBuffer += ss.str();
	for (HeaderMap::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
		this->_outBuffer += it->first;
		this->_outBuffer += ": ";
		this->_outBuffer += it->second;
		this->_outBuffer += "\r\n";
	}
	this->_outBuffer += "\r\n";
}

bool HttpMessage::outCompleted() const {
	return this->_outState == HttpMessage::SEND_COMPLETED;
}
