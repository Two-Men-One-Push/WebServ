#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "http/HttpMessage.hpp"
#include <istream>
#include <string>

class HttpRequest : public HttpMessage {
  private:
	std::string _method;
	std::string _path;

  protected:
	// Each one of the functions below return if they had enough content to finish their task
	bool appendMessageTypes(std::istream &input);

  public:
	HttpRequest();
	HttpRequest(const HttpRequest &other);
	HttpRequest &operator=(const HttpRequest &other);
	~HttpRequest();
};

#endif
