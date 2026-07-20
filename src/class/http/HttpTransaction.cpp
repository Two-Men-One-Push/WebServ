#include "./HttpTransaction.hpp"
#include "CGI/CGIInterface.hpp"
#include "Ressource/Ressource.hpp"
#include "Router/Router.hpp"
#include "WebServer/WebServer.hpp"
#include "http/errors/HttpErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "model/Server/Server.hpp"
#include <iostream>

HttpTransaction::HttpTransaction(const Server &serverConfig) : _serverConfig(serverConfig), _request(), _response() {
	std::cout << "New HTTP transaction created" << std::endl;
}

HttpTransaction::HttpTransaction(const HttpTransaction &other) : _serverConfig(other._serverConfig), _request(other._request), _response(other._response) {}

HttpTransaction::~HttpTransaction() {}

bool HttpTransaction::recvRequest(std::istream &input, WebServer &server) {
	try {
		bool result = this->_request.recvFrom(input);

		if (this->_request.waitingRouting()) {
			const Ressource ressource = Router::resolveRessource(this->_request, this->_serverConfig);
			try {
				this->handleRessource(ressource, server);
			} catch (const HttpError &e) {
				Ressource errorRessource = Router::resolveErrorRessource(this->_request, e.status(), this->_serverConfig);

				try {
					this->handleErrorRessource(errorRessource);
				} catch (const HttpError &e) {
					this->_response.generate(ressource.responseCode());
				}
			}
			this->_request.completeRouting();
			result = this->_request.recvFrom(input);
		}
		return result;
	} catch (const HttpError &e) {
		this->error(e);
		return true;
	} catch (const HttpMessage::Exception &e) {
		this->error(HttpError(e.requestStatus()));
		return true;
	}
}

void HttpTransaction::handleRessource(const Ressource &ressource, WebServer &server) {
	switch (ressource.type()) {
	case RESSOURCE_STATIC_FILE:
		this->_response.file(ressource.path(), ressource.responseCode(), ressource.mimeType());
		break;
	case RESSOURCE_REDIRECT:
		this->_response.redirect(ressource.path(), ressource.responseCode());
		break;
	case RESSOURCE_CGI:
		this->_response.cgi(*new CGIInterface(ressource, *this, server));
		break;
	case RESSOURCE_AUTO_INDEX:
		this->_response.autoIndex(ressource.path(), ressource.responseCode());
		break;
	case RESSOURCE_ERROR:
		if (ressource.path().empty()) {
			this->_response.generate(ressource.responseCode());
		} else {
			this->_response.file(ressource.path(), ressource.responseCode(), ressource.mimeType());
		}
		break;
	default:
		_exit(1);
	}
}

void HttpTransaction::handleErrorRessource(const Ressource &errorRessource) {
	if (errorRessource.path().empty()) {
		this->_response.generate(errorRessource.responseCode());
	} else {
		try {
			this->_response.file(errorRessource.path(), errorRessource.responseCode(), errorRessource.mimeType());
		} catch (const HttpError &e) {
			this->_response.generate(e.status());
		}
	}
}

bool HttpTransaction::recvResponse(std::istream &input) {
	try {
		return this->_response.recvFrom(input);
	} catch (const HttpError &e) {
		this->error(e);
		return true;
	} catch (const HttpMessage::Exception &e) {
		this->error(HttpError(e.responseStatus()));
		return true;
	}
}

bool HttpTransaction::sendRequest(const Fd &output) {
	return this->_request.sendTo(output);
}

bool HttpTransaction::sendRequestBody(const Fd &output) {
	return this->_response.sendBody(output);
}

bool HttpTransaction::sendResponse(const Fd &output) {
	return this->_response.sendTo(output);
}

const HttpRequest &HttpTransaction::request() const {
	return this->_request;
}

const HttpResponse &HttpTransaction::response() const {
	return this->_response;
}

void HttpTransaction::closeRequestInput() {
	try {
		this->_request.closeInput();
	} catch (const HttpError &e) {
		this->error(e);
	}
}

void HttpTransaction::closeResponseInput() {
	try {
		this->_response.closeInput();
	} catch (const HttpError &e) {
		this->error(e);
	}
}

void HttpTransaction::error(const HttpError &e) {
	const Ressource errorRessource = Router::resolveErrorRessource(this->_request, e.status(), this->_serverConfig);
	this->handleErrorRessource(errorRessource);
	this->_response.keepAlive(false);
}

bool HttpTransaction::keepAlive() const {
	return this->_request.keepAlive() && this->_response.keepAlive();
}

void HttpTransaction::kill() {
	this->_response.keepAlive(false);
}
