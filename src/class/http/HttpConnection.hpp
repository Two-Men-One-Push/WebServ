#ifndef HTTPCONNECTION_HPP
#define HTTPCONNECTION_HPP

#include "http/messages/HttpRequest.hpp"
#include "http/messages/HttpResponse.hpp"

class HttpConnection {
  private:
	HttpRequest _request;
	HttpResponse _response;

	bool _close;

  public:
	HttpConnection();
	HttpConnection(const HttpConnection &other);
	// HttpConnection &operator=(const HttpConnection &other);
	~HttpConnection();

	HttpRequest &request();
	const HttpRequest &request() const;
	HttpResponse &response();
	const HttpResponse &response() const;

	bool close() const;
	void close(bool close);
};

#endif
