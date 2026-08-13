#include "http/HttpStatus.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "http/types.hpp"
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

	headers["Date"] = "IL FAUT METTRE LA DATE";
	if (this->_version == HTTP1_1) {
		headers["Connection"] = this->_keepAlive ? "keep-alive" : "close";
	}
	if (this->_status != HttpStatus::NoContent && this->_status != HttpStatus::NotModified) {
		ss << this->_contentLength;
		headers["Content-Length"] = ss.str();
	}
	if (this->hasBody() && !this->_mimeType.empty()) {
		headers["Content-Type"] = this->_mimeType;
	}
	if (!this->_location.empty()) {
		headers["Location"] = this->_location;
	}
	if (this->_status == HttpStatus::MethodNotAllowed) {
		headers["Allow"] = HttpRequest::getAllowHeader();
	}
}
