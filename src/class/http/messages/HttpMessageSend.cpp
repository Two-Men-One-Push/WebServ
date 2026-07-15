#include "errors/WebservErrors.hpp"
#include "http/messages/Body/IBody.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <sstream>

bool HttpMessage::sendTo(const Fd &output) {
	switch (this->_outState) {
	case SEND_PREPARE_HEAD:
		this->prepareHeaders();
		this->_outState = SEND_HEAD;
		// fallthrough
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
		if (!this->sendBody(output)) return false;
		if (this->_sentSize == this->_contentLength) {
			delete this->_body;
			this->_body = NULL;
			this->_outState = SEND_COMPLETED;
		} else if (this->_bodyEmpty) {
		}
		// fallthrough
	case SEND_COMPLETED:
		return true;
	}
	return this->_outState == SEND_COMPLETED;
}

bool HttpMessage::sendHead(const Fd &output) {
	std::string &buffer = this->_outBuffer;

	if (buffer.empty()) this->formatHead();
	ssize_t sent = output.write(buffer.data(), std::min(buffer.size(), (size_t)WRITE_SIZE));
	std::cerr << "\e[0;32m";
	std::cerr.write(buffer.data(), sent);
	std::cerr << "\e[0m\n";
	buffer.erase(0, sent);
	return buffer.empty();
}

bool HttpMessage::sendBody(const Fd &output) {
	std::string &buffer = this->_outBuffer;
	IBody &body = *this->_body;

	size_t toWrite = std::min(_contentLength, (size_t)WRITE_SIZE);

	if (buffer.size() < toWrite && !this->_bodyEmpty) {
		size_t oldSize = buffer.size();
		size_t missingSize = toWrite - oldSize;

		buffer.resize(toWrite);
		ssize_t readSize = body.read(&buffer[0] + oldSize, missingSize);

		if (readSize < 0) throw WebservErrors::SysError("read", errno);
		if (readSize == 0) this->_bodyEmpty = true;

		buffer.resize(oldSize + readSize);
	}

	ssize_t sent = output.write(buffer.data(), buffer.size());
	if (sent < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false;
		throw WebservErrors::SysError("write", errno);
	}
	std::cerr << "\e[0;32m";
	std::cerr.write(buffer.data(), sent);
	std::cerr << "\e[0m\n";
	this->_sentSize += sent;
	buffer.erase(0, sent);

	if (this->_sentSize == this->_contentLength) {
		return true;
	} else if (buffer.empty() && this->_bodyEmpty) {
		throw WebservErrors::Runtime("Not enough body to send");
	} else {
		return false;
	}
}

void HttpMessage::formatHead() {
	std::stringstream ss;

	this->formatTypeLine();

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
