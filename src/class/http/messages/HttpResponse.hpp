#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/HttpStatus.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/HttpMessage.hpp"

class HttpConnection;

class HttpResponse : public HttpMessage {
  private:
	HttpResponse(const HttpResponse &other);
	HttpResponse &operator=(const HttpResponse &other);

	HttpStatus::Code _status;
	std::string _message;

  protected:
	bool hasBody() const;
	bool appendMessageTypes(std::istream &input);

  public:
	HttpResponse(HttpConnection &connection);
	HttpResponse(const HttpResponse &other, HttpConnection &connection);
	~HttpResponse();

	void error(const HttpException &e);

	int status() const;
	void status(int status);
};

#endif
