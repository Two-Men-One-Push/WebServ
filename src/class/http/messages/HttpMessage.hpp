#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "http/types.hpp"
#include <cstddef>
#include <istream>
#include <string>

#define TMP_HTTP_BUFFER_SIZE 8192 /* !:! tmp en attendant la config */

class HttpConnection;

class HttpMessage {
  private:
	HttpMessage &operator=(const HttpMessage &other);

	// parsing data

	enum ParsingState {
		MESSAGE_TYPES,
		MESSAGE_HEADERS,
		MESSAGE_USED_HEADERS,
		MESSAGE_BODY,
		COMPLETED,
	};

	ParsingState _state;

	// Each one of the functions below return if they had enough content to finish their task
	bool appendMessageHeaders(std::istream &input);

  protected:
	// HTTP message data
	static HttpVersion parseHttpVersion(const std::string &input);

	HttpVersion _version;
	Headers _headers;
	std::string _buffer;

	size_t _contentLength;
	TransferEncoding _transferEncoding;

	HttpConnection &_connection;

	virtual bool appendMessageTypes(std::istream &input) = 0;
	void loadBaseUsedHeaders();

	// HEADER LOADER

	void loadTranferEncoding();

	virtual void loadTypeUsedHeaders() = 0;
	bool collectBody(std::istream &input);

	virtual bool hasBody() const;

	bool extractMessageHeaders(std::istream &input);

  public:
	HttpMessage(HttpConnection &connection);
	HttpMessage(const HttpMessage &other, HttpConnection &connection);
	virtual ~HttpMessage();

	HttpVersion version() const;
	std::string versionStr() const;

	void setHeader(const std::string &fieldName, const std::string &fieldValue);

	bool completed() const;
	void state(ParsingState state);

	bool append(std::istream &input);
	void end();

	const Headers &headers() const;
	Headers &headers();

	TransferEncoding transferEncoding() const;
	std::string transferEncodingStr() const;
};

#endif
