#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "AFd/AFd.hpp"
#include "http/types.hpp"
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

#define WRITE_SIZE 4096

#define TMP_HTTP_BUFFER_SIZE 8192 /* !:! tmp en attendant la config */

class HttpMessage {
  private:
	HttpMessage &operator=(const HttpMessage &other);

	// RECEIVE

	enum InState {
		RECV_MESSAGE_TYPES,
		RECV_MESSAGE_HEADERS,
		RECV_LOAD_MESSAGE_HEADERS,
		RECV_MESSAGE_BODY,
		RECV_COMPLETED,
	};

	InState _inState;
	size_t _readSize;

	bool recvMessageHeaders(std::istream &input);
	bool recvBody(std::istream &input);

	bool collectRawBody(std::istream &input);
	bool extractMessageHeaders(std::istream &input);

	// HEADER LOADER

	void loadTranferEncoding();
	void loadContentLength();

	// SEND

	enum OutState {
		SEND_HEAD,
		SEND_BODY,
		SEND_COMPLETED,
	};

	OutState _outState;
	std::size_t _sentSize;
	bool _bodyEmpty;

	bool sendHead(const AFd &output);

	void formatHead();

  protected:
	// HTTP message data

	HttpVersion _version;
	HeaderMap _headers;
	std::iostream *_body;

	std::string _inBuffer;
	std::string _outBuffer;

	size_t _contentLength;
	TransferEncoding _transferEncoding;

	virtual bool recvTypeLine(std::istream &input) = 0;
	void loadBaseUsedHeaders();


	virtual void loadTypeUsedHeaders() = 0;

	virtual void formatTypeLine() = 0;

	static HttpVersion parseHttpVersion(const std::string &input);

  public:
	HttpMessage();
	HttpMessage(const HttpMessage &other);
	virtual ~HttpMessage();

	HttpVersion version() const;
	std::string versionStr() const;

	void setHeader(const std::string &fieldName, const std::string &fieldValue);

	virtual bool hasBody() const;

	bool recvFrom(std::istream &input);
	void inState(InState state);
	bool inCompleted() const;

	bool sendTo(const AFd &output);
	bool sendBody(const AFd &output);
	void outState(InState state);
	bool outCompleted() const;

	void end();

	const HeaderMap &headers() const;
	HeaderMap &headers();

	size_t contentLength() const { return _contentLength; }

	TransferEncoding transferEncoding() const;
	std::string transferEncodingStr() const;

	virtual std::ostream &printTypeInfo(std::ostream &os) const = 0;
	std::ostream &print(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const HttpMessage &m);

#endif
