#ifndef HTTPTRANSACTION_HPP
#define HTTPTRANSACTION_HPP

#include "Ressource/Ressource.hpp"
#include "http/errors/HttpErrors.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "model/Server/Server.hpp"
#include <istream>

class WebServer;

class HttpTransaction {
  private:
	const Server &_serverConfig;
	HttpRequest _request;
	HttpResponse _response;

	void handleRessource(const Ressource &ressource, WebServer &server);
	void handleErrorRessource(const Ressource &ressource);

  public:
	HttpTransaction(const Server &serverConfig);
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
