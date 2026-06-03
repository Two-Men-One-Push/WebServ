#include "./HttpMessage.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "http/types.hpp"

HttpMessage::HttpMessage(HttpTransaction &connection)
	: _state(HttpMessage::MESSAGE_TYPES),
	  _version(),
	  _headers(),
	  _body(),
	  _buffer(),
	  _contentLength(0),
	  _transferEncoding(TE_UNDEFINED),
	  _transaction(connection),
	  _readContentLength(0) {
	this->_buffer.reserve(TMP_HTTP_BUFFER_SIZE);
}

HttpMessage::HttpMessage(const HttpMessage &other, HttpTransaction &connection)
	: _state(other._state),
	  _version(other._version),
	  _headers(other._headers),
	  _body(other._body),
	  _buffer(other._buffer),
	  _contentLength(other._contentLength),
	  _transferEncoding(other._transferEncoding),
	  _transaction(connection),
	  _readContentLength(other._readContentLength) {}

HttpMessage::~HttpMessage() {}

HttpVersion HttpMessage::version() const {
	return this->_version;
}

std::string HttpMessage::versionStr() const {
	return httpVersionString(this->_version);
}

void HttpMessage::setHeader(const std::string &fieldName, const std::string &fieldValue) {
	this->_headers[fieldName] = fieldValue;
}

const HeaderMap &HttpMessage::headers() const {
	return this->_headers;
}

HeaderMap &HttpMessage::headers() {
	return this->_headers;
}

TransferEncoding HttpMessage::transferEncoding() const {
	return this->_transferEncoding;
}

std::string HttpMessage::transferEncodingStr() const {
	return transferEncodingString(this->_transferEncoding);
}

std::ostream &operator<<(std::ostream &os, const HttpMessage &m) {
	return m.print(os);
}

std::ostream &HttpMessage::print(std::ostream &os) const {
	const HeaderMap &headers = this->_headers;

	this->printTypeInfo(os);

	if (headers.has("Tranfer-Encoding"))
		os << "tranfer_encoding = " << this->transferEncodingStr() << '\n';
	if (headers.has("Content-Length"))
		os << "content_length = " << this->_contentLength << '\n';

	for (HeaderMap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
		os << '"' << it->first << "\": \"" << it->second << "\"";
		if (it != --headers.end()) os << '\n';
	}
	if (this->hasBody()) os << this->_body;
	return os;
}
