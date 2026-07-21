#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "URL/URL.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <cstddef>
#include <istream>
#include <map>
#include <string>
#include <sys/socket.h>

class HttpRequest : public HttpMessage {
  private:
	HttpRequest &operator=(const HttpRequest &other);

	HttpMethod _method;
	URL _uri;

	enum FirstLineParsingState {
		REQUEST_METHOD,
		REQUEST_URI,
		REQUEST_VERSION,
	};

	FirstLineParsingState _firstLineState;

	size_t _maxMethodSize;
	static size_t _maxUriSize;
	static size_t _maxVersionSize;

	size_t getMaxMethodSize();

	bool parseRequestMethod(std::istream &input);
	bool parseRequestUri(std::istream &input);
	bool parseRequestVersion(std::istream &input);

  protected:
	// Each one of the functions below return if they had enough content to finish their task
	bool recvTypeLine(std::istream &input);
	void loadTypeUsedHeaders();
	void checkBodyType();

	void prepareHeaders();
	void formatTypeLine();

  public:
	HttpRequest();
	HttpRequest(const HttpRequest &other);
	~HttpRequest();

	HttpMethod method() const;
	void method(const HttpMethod &newMethod);
	std::string methodStr() const;

	const URL &uri() const;
	void uri(const URL &newUri);

	void closeInput();

	std::ostream &
	printTypeInfo(std::ostream &os) const;

	static std::map<std::string, HttpMethod> implementedHttpMethod;
	static std::string getAllowHeader();
};

#endif
