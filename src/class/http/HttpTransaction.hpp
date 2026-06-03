#ifndef HTTPCONNECTION_HPP
#define HTTPCONNECTION_HPP

#include "http/errors/HttpException.hpp"
#include "http/messages/HttpRequest.hpp"
#include "http/messages/HttpResponse.hpp"

class HttpTransaction {
  private:
	HttpRequest _request;
	HttpResponse _response;

	bool _last;

  public:
	HttpTransaction();
	HttpTransaction(const HttpTransaction &other);
	// HttpConnection &operator=(const HttpConnection &other);
	~HttpTransaction();

	HttpRequest &request();
	const HttpRequest &request() const;
	HttpResponse &response();
	const HttpResponse &response() const;

	void error(const HttpException &e);
};

#endif
