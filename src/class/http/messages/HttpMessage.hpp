#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "http/types.hpp"
#include <cstddef>
#include <istream>

#define HTTP_BUFFER_SIZE 4096 /* !:! tmp en attendant la config */

class HttpConnection;

class HttpMessage {
  private:
	HttpMessage &operator=(const HttpMessage &other);

	// parsing data

	typedef enum {
		MESSAGE_TYPES,
		MESSAGE_HEADERS,
		MESSAGE_BODY,
		COMPLETED,
	} ParsingState;

	ParsingState _state;

	// Each one of the functions below return if they had enough content to finish their task
	virtual bool appendMessageTypes(std::istream &input) = 0;
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
};

#endif
