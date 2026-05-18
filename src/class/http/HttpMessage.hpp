#ifndef HTTPMESSAGE_HPP
#define HTTPMESSAGE_HPP

#include "http/types.hpp"
#include <istream>

class HttpMessage {
  private:
	// HTTP message data

	Headers _headers;

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
	std::string _version;
	std::string _buffer;

	virtual bool hasBody() const;

  public:
	HttpMessage();
	HttpMessage(const HttpMessage &other);
	HttpMessage &operator=(const HttpMessage &other);
	virtual ~HttpMessage();

	bool completed() const;

	bool append(std::istream &input);

	Headers &headers() const;
};

#endif
