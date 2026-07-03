#include "./HttpMessage.hpp"
#include "http/types.hpp"

HttpMessage::HttpMessage()
	: _readSize(0),
	  _chunkInfo((BodyChunkInfo){BodyChunkInfo::CHUNK_SIZE, 0, 0}),
	  _outState(SEND_HEAD),
	  _sentSize(0),
	  _bodyEmpty(false),
	  _version(HTTP1_1),
	  _headers(),
	  _body(NULL),
	  _inState(HttpMessage::RECV_MESSAGE_TYPES),
	  _inBuffer(),
	  _outBuffer(),
	  _contentLength(0),
	  _transferEncoding(TE_UNDEFINED) {
	this->_inBuffer.reserve(TMP_HTTP_BUFFER_SIZE);
}

HttpMessage::HttpMessage(const HttpMessage &other)
	: _readSize(other._readSize),
	  _chunkInfo(other._chunkInfo),
	  _outState(other._outState),
	  _sentSize(other._sentSize),
	  _bodyEmpty(other._bodyEmpty),
	  _version(other._version),
	  _headers(other._headers),
	  _body(other._body),
	  _inState(other._inState),
	  _inBuffer(other._inBuffer),
	  _outBuffer(other._outBuffer),
	  _contentLength(other._contentLength),
	  _transferEncoding(other._transferEncoding) {}

// HttpMessage &HttpMessage::operator=(const HttpMessage &other) {
// 	if (this != &other) {
// 		this->_state = other._state;
// 		this->_version = other._version;
// 		this->_headers = other._headers;
// 		this->_body = other._body;
// 		this->_buffer = other._buffer;
// 		this->_contentLength = other._contentLength;
// 		this->_transferEncoding = other._transferEncoding;
// 		this->_readContentLength = other._readContentLength;
// 	}
// 	return *this;
// }

HttpMessage::~HttpMessage() {
	delete this->_body;
}

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

void HttpMessage::end() {
	this->_inState = HttpMessage::RECV_COMPLETED;
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
		os << it->first << ": " << it->second << "\r\n";
	}
	if (this->hasBody()) os << this->_body;
	return os;
}
