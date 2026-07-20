#ifndef HTTPTRANSACTION_HPP
#define HTTPTRANSACTION_HPP

#include "http/errors/HttpErrors.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <istream>

class HttpTransaction {
  private:
	HttpRequest _request;
	HttpResponse _response;

  public:
	HttpTransaction();
	HttpTransaction(const HttpTransaction &other);
	HttpTransaction &operator=(const HttpTransaction &other);
	~HttpTransaction();

	const HttpRequest &request() const;
	const HttpResponse &response() const;

	bool recvRequest(std::istream &input, WebServer &server);
	bool recvResponse(std::istream &input);

	bool sendRequest(const Fd &output);
	bool sendRequestBody(const Fd &output);
	bool sendResponse(const Fd &output);

	void closeRequestInput();
	void closeResponseInput();

	void error(const HttpError &e);

	bool keepAlive() const;
	void kill();
};

#endif
