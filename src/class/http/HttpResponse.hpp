#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/HttpMessage.hpp"

class HttpResponse : public HttpMessage {
  private:
	int _status;

  protected:
	bool appendMessageTypes(std::istream &input);

  public:
	HttpResponse();
	HttpResponse(const HttpResponse &other);
	HttpResponse &operator=(const HttpResponse &other);
	~HttpResponse();

	int status() const;
	void status(int status);
};

#endif
