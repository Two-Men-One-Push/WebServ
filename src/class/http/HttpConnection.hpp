#ifndef HTTPCONNECTION_HPP
#define HTTPCONNECTION_HPP

#include "http/errors/HttpException.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/messages/HttpResponse.hpp"

class HttpConnection {
  private:
	HttpRequest _request;
	HttpResponse _response;

	bool _last;

  public:
	HttpConnection();
	HttpConnection(const HttpConnection &other);
	// HttpConnection &operator=(const HttpConnection &other);
	~HttpConnection();

	HttpRequest &request();
	const HttpRequest &request() const;
	HttpResponse &response();
	const HttpResponse &response() const;

	void error(const HttpException &e);
};

#endif
