#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "http/types.hpp"
#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

#define TMP_HTTP_BUFFER_SIZE 8192 /* !:! tmp en attendant la config */

class HttpTransaction;

class HttpMessage {
  private:
	HttpMessage &operator=(const HttpMessage &other);

	// parsing data

	enum ParsingState {
		MESSAGE_TYPES,
		MESSAGE_HEADERS,
		LOAD_MESSAGE_HEADERS,
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
	HeaderMap _headers;
	std::string _body;

	std::string _buffer;

	size_t _contentLength;
	TransferEncoding _transferEncoding;

	HttpTransaction &_transaction;

	virtual bool appendMessageTypes(std::istream &input) = 0;
	void loadBaseUsedHeaders();

	size_t _readContentLength;

	// HEADER LOADER

	void loadTranferEncoding();
	void loadContentLenght();

	virtual void loadTypeUsedHeaders() = 0;
	bool collectBody(std::istream &input);
	bool collectRawBody(std::istream &input);


	bool extractMessageHeaders(std::istream &input);

  public:
	HttpMessage(HttpTransaction &connection);
	HttpMessage(const HttpMessage &other, HttpTransaction &connection);
	virtual ~HttpMessage();

	HttpVersion version() const;
	std::string versionStr() const;

	void setHeader(const std::string &fieldName, const std::string &fieldValue);

	virtual bool hasBody() const;

	bool completed() const;
	void state(ParsingState state);

	bool append(std::istream &input);
	void end();

	const HeaderMap &headers() const;
	HeaderMap &headers();

	TransferEncoding transferEncoding() const;
	std::string transferEncodingStr() const;

	virtual std::ostream &printTypeInfo(std::ostream &os) const = 0;
	std::ostream &print(std::ostream &os) const;
};

std::ostream &operator<<(std::ostream &os, const HttpMessage &m);

#endif
