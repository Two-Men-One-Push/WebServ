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

	enum FirstLineParsingState {
		RESPONSE_VERSION,
		RESPONSE_STATUS,
		RESPONSE_MESSAGE,
	};

	FirstLineParsingState _firstLineState;

	static size_t _maxVersionSize;
	static size_t _maxStatusSize;
	static size_t _maxMessageSize;

	size_t getMaxMethodSize();

	bool parseResponseVersion(std::istream &input);
	bool parseResponseStatus(std::istream &input);
	bool parseResponseMessage(std::istream &input);

	CGIInterface *_cgiInterface;

	void formatHead();
	void loadCGIStatus(const std::string &statusString);

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

	void closeInput();

	void prepareHeaders();

	void checkBodyType();

	void file();
	void cgi(CGIInterface &cgi);
	void error(const HttpException &e);

	bool formatCompleted();

	std::ostream &printTypeInfo(std::ostream &os) const;
};

#endif
