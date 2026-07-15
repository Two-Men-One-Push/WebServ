#include "http/errors/HttpStandardException.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"
#include <exception>

void HttpMessage::loadTranferEncoding() {
	HeaderMap &headers = this->_headers;
	if (!headers.has("Transfer-Encoding")) return;
	if (headers.at("Transfer-Encoding") != "chunked") throw HttpExceptions::NotImplementedException();
	this->_transferEncoding = TE_CHUNKED;
}

void HttpMessage::loadContentLength() {
	HeaderMap &headerMap = this->_headers;
	if (!headerMap.has("Content-Length")) return;
	if (this->_transferEncoding != TE_UNDEFINED) throw HttpExceptions::BadRequestException();
	this->_inputWillClose = false;
	try {
		this->_contentLength = parseULong(headerMap.at("Content-Length"));
	} catch (const std::exception &) {
		throw HttpExceptions::BadRequestException();
	}
}

void HttpMessage::loadConnection() {
	HeaderMap &headerMap = this->_headers;
	if (!headerMap.has("Connection")) return;
	if (headerMap.at("Connection") == "keep-alive") {
		this->_inputWillClose = false;
		_contentLength = 0;
	}
}
