#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <istream>
#include <map>
#include <string>

class HttpRequest : public HttpMessage {
  private:
	HttpRequest &operator=(const HttpRequest &other);

	HttpMethod _method;
	std::string _uri;

	bool extractRequestLine(std::istream &input);
	static HttpMethod parseRequestMethod(const std::string &input);

  protected:
	// Each one of the functions below return if they had enough content to finish their task
	bool appendMessageTypes(std::istream &input);

  public:
	static std::map<std::string, HttpMethod> implementedHttpMethod;
	static std::string getAllowHeader();

	HttpRequest(HttpConnection &connection);
	HttpRequest(const HttpRequest &other, HttpConnection &connection);
	~HttpRequest();

	HttpMethod method() const;
	std::string methodStr() const;

	const std::string &uri() const;
};

#endif
