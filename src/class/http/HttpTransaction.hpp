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

	bool _isLast;

  public:
	HttpTransaction();
	HttpTransaction(const HttpTransaction &other);
	HttpTransaction &operator=(const HttpTransaction &other);
	~HttpTransaction();

	const HttpRequest &request() const;
	const HttpResponse &response() const;

	bool recvRequest(std::istream &input, WebServer &server);
	bool recvResponse(std::istream &input);

	bool sendRequest(const AFd &output);
	bool sendRequestBody(const AFd &output);
	bool sendResponse(const AFd &output);

	void error(const HttpException &e);

	bool isLast() const;
	void isLast(bool isLast);
};

#endif
