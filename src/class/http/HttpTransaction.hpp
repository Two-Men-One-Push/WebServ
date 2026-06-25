#ifndef HTTPTRANSACTION_HPP
#define HTTPTRANSACTION_HPP

#include "http/errors/HttpException.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <istream>

class HttpTransaction {
  private:
	HttpRequest _request;
	HttpResponse _response;

	bool _last;

  public:
	HttpTransaction();
	HttpTransaction(const HttpTransaction &other);
	HttpTransaction &operator=(const HttpTransaction &other);
	~HttpTransaction();

	const HttpRequest &request() const;
	const HttpResponse &response() const;

	bool appendToRequest(std::istream &input);
	bool appendToResponse(std::istream &input);

	void error(const HttpException &e);
};

#endif
