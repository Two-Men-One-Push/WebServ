#ifndef HTTPTRANSACTION_HPP
#define HTTPTRANSACTION_HPP

#include "http/errors/HttpException.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"

class HttpTransaction {
  private:
	HttpRequest _request;
	HttpResponse _response;

	bool _last;

  public:
	HttpTransaction();
	HttpTransaction(const HttpTransaction &other);
	// HttpTransaction &operator=(const HttpTransaction &other);
	~HttpTransaction();

	HttpRequest &request();
	const HttpRequest &request() const;
	HttpResponse &response();
	const HttpResponse &response() const;

	void error(const HttpException &e);
};

#endif
