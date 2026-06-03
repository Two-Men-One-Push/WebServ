#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "http/HttpStatus.hpp"
#include "http/errors/HttpException.hpp"
#include "http/messages/HttpMessage.hpp"

#define TMP_SERVER_ROOT "www"

class HttpTransaction;
class ClientSocket;

class HttpResponse : public HttpMessage {
  private:
	HttpResponse(const HttpResponse &other);
	HttpResponse &operator=(const HttpResponse &other);

	HttpStatus::Code _status;
	std::string _message;

  protected:
	bool appendMessageTypes(std::istream &input);

	void loadTypeUsedHeaders();

  public:
	HttpResponse(HttpTransaction &connection);
	HttpResponse(const HttpResponse &other, HttpTransaction &connection);
	~HttpResponse();

	int status() const;
	void status(int status);

	bool hasBody() const;

	void error(const HttpException &e);

	/** @return true if the full predicted content was sent, and false otherwise. */
	bool send(ClientSocket &clientSocket);

	std::ostream &printTypeInfo(std::ostream &os) const;
};

#endif
