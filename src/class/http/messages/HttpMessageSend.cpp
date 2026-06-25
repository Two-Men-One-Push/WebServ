#include "errors/WebservErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include <sstream>


bool HttpMessage::send(AFd &output) {
	ssize_t sentSize = output.write(this->_outBuffer.data(), this->_outBuffer.size());
	if (sentSize == -1) throw WebservErrors::SysError("ClientSocket write", errno);
	return true;
	switch (this->_outState) {
	case SEND_HEAD:
		if (!this->sendHead(output)) return false;
		this->_outState = SEND_BODY;
		// fallthrough
	case SEND_BODY:
		if (!this->sendBody(output)) return false;
		// fallthrough
		this->_outState = SEND_COMPLETED;
	case SEND_COMPLETED:
		return true;
	}
}

bool HttpMessage::sendHead(AFd &output) {
	std::string &buffer = this->_outBuffer;

	if (buffer.empty()) this->formatHead();
	ssize_t sent = output.write(buffer.data(), WRITE_SIZE);
	buffer.erase(0, sent);
	return buffer.empty();
}

bool HttpMessage::sendBody(AFd &output) {
	std::string &buffer = this->_outBuffer;

	if (buffer.size() < WRITE_SIZE && !this->_bodyEmpty) {
		size_t oldSize = buffer.size();
		buffer.resize(WRITE_SIZE);
		size_t missingSize = WRITE_SIZE - oldSize;
		std::streamsize readSize = this->_body->readsome(&buffer[0] + oldSize, missingSize);
		buffer.resize(oldSize + (size_t)readSize);
		if (readSize == 0 && this->_body->eof())
			this->_bodyEmpty = true;
	}

	if (buffer.empty())
		return this->_bodyEmpty;

	ssize_t sent = output.write(buffer.data(), buffer.size());
	if (sent < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return false; // pas prêt, on retentera au prochain EPOLLOUT
		throw WebservErrors::SysError("write", errno);
	}
	buffer.erase(0, sent);
	return buffer.empty() && this->_bodyEmpty;
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
