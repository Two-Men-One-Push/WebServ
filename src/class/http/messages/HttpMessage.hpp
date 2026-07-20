#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "Fd/Fd.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/IBody.hpp"
#include "http/types.hpp"
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>
#include <sys/types.h>

#define WRITE_SIZE 4096
#define READ_SIZE 4096

#define TMP_HTTP_BUFFER_SIZE 8192 /* !:! tmp en attendant la config */

struct BodyChunkInfo {
	enum {
		CHUNK_SIZE,
		CHUNK_CONTENT,
		CHUNK_CRLF,
		CHUNK_TRAILER,
		CHUNK_COMPLETED,
	} state;

	std::size_t size;
	std::size_t readSize;
};

class HttpMessage {
  private:
	HttpMessage &operator=(const HttpMessage &other);

	// RECEIVE

	bool recvMessageHeaders(std::istream &input);
	bool recvBody(std::istream &input);

	void writeBody(const char *buffer, size_t size);
	bool collectRawBody(std::istream &input);
	bool collectRawBodyToEOF(std::istream &input);
	bool collectChunkedBody(std::istream &input);

	// HEADER LOADER

	void loadTranferEncoding();
	void loadContentLength();
	void loadConnection();

	// BODY LOADING INFO

	size_t _readSize;
	BodyChunkInfo _chunkInfo;
	bool getChunkSize(std::istream &input);
	bool getChunkContent(std::istream &input);
	bool getChunkCrlf(std::istream &input);
	bool getChunkedTrailer(std::istream &input);

	// SEND

	enum OutState {
		SEND_PREPARE_HEAD,
		SEND_HEAD,
		SEND_BODY,
		SEND_COMPLETED,
	};

	OutState _outState;
	std::pair<std::string, std::string> _bufferedHeaderField;
	std::size_t _sentSize;
	bool _bodyEmpty;

	bool sendHead(const Fd &output);

	void formatHead();

  protected:
	// HTTP message data

	HttpVersion _version;
	HeaderMap _headers;
	IBody *_body;

	enum InState {
		RECV_MESSAGE_TYPES,
		RECV_MESSAGE_HEADERS,
		RECV_LOAD_MESSAGE_HEADERS,
		RECV_MESSAGE_BODY,
		RECV_COMPLETED,
	};

	InState _inState;
	std::string _inBuffer;
	std::string _outBuffer;

	size_t _contentLength;
	bool _keepAlive;
	TransferEncoding _transferEncoding;

	enum {
		BT_NONE,
		BT_CONTENT_LENGTH,
		BT_CHUNKED,
		BT_EOF,
	} _bodyType;

	virtual bool recvTypeLine(std::istream &input) = 0;
	void loadBaseUsedHeaders();

	virtual void loadTypeUsedHeaders() = 0;
	virtual void checkBodyType();

	virtual void prepareHeaders() = 0;
	virtual void formatTypeLine() = 0;

	static HttpVersion parseHttpVersion(const std::string &input);

  public:
	class Exception : WebservErrors::Runtime {
		HttpStatus::Code _requestExpect;
		HttpStatus::Code _responseExpect;

	  public:
		Exception() throw()
			: WebservErrors::Runtime(""),
			  _requestExpect(HttpStatus::BadRequest),
			  _responseExpect(HttpStatus::BadGateway) {}

		Exception(const std::string &message) throw()
			: WebservErrors::Runtime(message),
			  _requestExpect(HttpStatus::BadRequest),
			  _responseExpect(HttpStatus::BadGateway) {}

		Exception(const Exception &other) throw()
			: WebservErrors::Runtime(other),
			  _requestExpect(other._requestExpect),
			  _responseExpect(other._responseExpect) {}

		HttpStatus::Code requestStatus() const { return this->_requestExpect; }

		Exception &requestStatus(HttpStatus::Code expected) {
			this->_requestExpect = expected;
			return *this;
		}

		HttpStatus::Code responseStatus() const { return this->_responseExpect; }

		Exception &responseStatus(HttpStatus::Code expected) {
			this->_responseExpect = expected;
			return *this;
		}
	};

	HttpMessage();

	HttpMessage(const HttpMessage &other);
	virtual ~HttpMessage();

	HttpVersion version() const;
	std::string versionStr() const;

	void setHeader(const std::string &fieldName, const std::string &fieldValue);

	bool hasBody() const;

	bool recvFrom(std::istream &input);
	void inState(InState state);
	bool inCompleted() const;

	bool sendTo(const Fd &output);
	bool sendBody(const Fd &output);
	void outState(InState state);
	bool outCompleted() const;

	void end();

	const HeaderMap &headers() const;
	HeaderMap &headers();

	size_t contentLength() const { return _contentLength; }

	TransferEncoding tranferEncording() const { return _transferEncoding; }

	bool keepAlive() const { return _keepAlive; }

	void keepAlive(bool value) { this->_keepAlive = value; }

	virtual std::ostream &printTypeInfo(std::ostream &os) const = 0;
	std::ostream &print(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const HttpMessage &m);

#endif
