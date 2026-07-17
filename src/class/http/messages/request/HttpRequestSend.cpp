#include "./HttpRequest.hpp"
#include <sstream>

void HttpRequest::formatTypeLine() {
	std::stringstream ss;

	ss << this->methodStr() << ' ' << this->_uri.raw() << ' ' << this->versionStr() << "\r\n";

	this->_outBuffer += ss.str();
}

void HttpRequest::prepareHeaders() {
	HeaderMap &headers = this->_headers;
	std::stringstream ss;

	if (this->_inputWillClose) {
		ss << this->_contentLength;
		headers["Content-Length"] = ss.str();
	}
}
