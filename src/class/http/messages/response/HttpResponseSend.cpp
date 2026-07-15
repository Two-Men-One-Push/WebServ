#include "http/messages/response/HttpResponse.hpp"
#include <iostream>
#include <sstream>
#include <sys/types.h>

void HttpResponse::formatTypeLine() {
	std::stringstream ss;

	ss << this->versionStr() << ' ' << this->_status << ' ' << this->_message << "\r\n";

	this->_outBuffer += ss.str();
}

void HttpResponse::prepareHeaders() {
	HeaderMap &headers = this->_headers;
	std::stringstream ss;

	if (this->_inputWillClose) {
		ss << this->_contentLength;
		headers["Content-Length"] = ss.str();
	}
	if (!headers.has("Connection") || headers.at("Connection") != "close") {
		headers["Connection"] = "keep-alive";
	}
}
