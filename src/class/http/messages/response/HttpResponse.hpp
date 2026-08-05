#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "CGI/CGIInterface.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/HttpMessage.hpp"
#include <istream>
#include <string>

class ClientSocket;

class HttpResponse : public HttpMessage {
  private:
	HttpResponse &operator=(const HttpResponse &other);

	HttpStatus::Code _status;
	std::string _message;

	std::string _location;

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

	void loadCGIStatus();

	CGIInterface *_cgiInterface;

	void formatHead();

  protected:
	// Each one of the functions below return if they had enough content to finish their task
	bool recvTypeLine(std::istream &input);
	void loadTypeHeaders();

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

	void file(const std::string &path, HttpStatus::Code status, const std::string &mimeType);
	void generate(HttpStatus::Code status);
	void generate(HttpStatus::Code status, const std::string &message);
	void redirect(const std::string &redirectUrl, HttpStatus::Code status);
	void autoIndex(const std::string &root, const std::string &path, HttpStatus::Code status);
	void cgi(CGIInterface &cgi);
	void uploaded(const std::string &uploadPath = "");
	void deleteFile(const std::string &path);

	bool formatCompleted();

	std::ostream &printTypeInfo(std::ostream &os) const;
};

#endif
