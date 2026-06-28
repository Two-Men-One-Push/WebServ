#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "CGI/CGIInterface.hpp"
#include "http/HttpStatus.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/HttpMessage.hpp"
#include <istream>

#define TMP_SERVER_ROOT "www"

class ClientSocket;

class HttpResponse : public HttpMessage {
  private:
	HttpResponse &operator=(const HttpResponse &other);

	HttpStatus::Code _status;
	std::string _message;

	CGIInterface *_cgiInterface;

	void formatHead();

  protected:
	// Each one of the functions below return if they had enough content to finish their task
	bool recvTypeLine(std::istream &input);
	void loadTypeUsedHeaders();

	void formatTypeLine();

  public:
	HttpResponse();
	HttpResponse(const HttpResponse &other);
	~HttpResponse();

	HttpStatus::Code status() const;
	void status(int status);

	bool hasBody() const;

	void file();
	void cgi(CGIInterface &cgi);
	void error(const HttpException &e);

	bool formatCompleted();

	std::ostream &printTypeInfo(std::ostream &os) const;
};

#endif
