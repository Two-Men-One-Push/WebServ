#include "./HttpTransaction.hpp"
#include "CGI/CGIInterface.hpp"
#include "Ressource/Ressource.hpp"
#include "WebServer/WebServer.hpp"
#include "http/errors/HttpErrors.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include "model/Server/Server.hpp"
#include "utils/formatting.hpp"
#include <exception>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

HttpTransaction::HttpTransaction(const Server &serverConfig, const struct sockaddr_storage &serverAddress, const struct sockaddr_storage &clientAddress)
	: _serverConfig(serverConfig),
	  _resolvedConfig(NULL),
	  _serverAddress(serverAddress),
	  _clientAddress(clientAddress),
	  _request(),
	  _response() {
	std::cerr << "HttpTransaction created" << std::endl;
}

HttpTransaction::HttpTransaction(const HttpTransaction &other)
	: _serverConfig(other._serverConfig),
	  _resolvedConfig(other._resolvedConfig),
	  _serverAddress(other._serverAddress),
	  _clientAddress(other._clientAddress),
	  _request(other._request),
	  _response(other._response) {}

HttpTransaction::~HttpTransaction() {
	std::cerr << "HttpTransaction deleted" << std::endl;
}

bool HttpTransaction::recvRequest(std::istream &input, WebServer &server) {
	try {
		bool result = this->_request.recvFrom(input, this->nearestConfig());

		if (this->_request.isWaitingRouting()) {
			const Ressource ressource(this->_request, this->_serverConfig);
			try {
				this->handleRessource(ressource, server);
			} catch (const HttpError &e) {
				Ressource errorRessource(this->_request, e.status(), this->_serverConfig);

				try {
					this->handleErrorRessource(errorRessource, e);
				} catch (const HttpError &e) {
					this->_response.generate(ressource.responseCode());
				} catch (const std::exception &e) {
					this->_response.generate(ressource.responseCode());
					throw;
				}
			}
			this->_request.completeRouting();
			result = this->_request.recvFrom(input, this->nearestConfig());
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
		this->_response.file(ressource.fullPath(), ressource.responseCode(), ressource.mimeType());
		break;
	case RESSOURCE_REDIRECT:
		this->_response.redirect(ressource.path(), ressource.responseCode());
		break;
	case RESSOURCE_CGI:
		try {
			this->_response.cgi(*new CGIInterface(ressource, *this, server));
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			throw HttpErrors::InternalServerErrorException();
		}
		break;
	case RESSOURCE_AUTO_INDEX:
		this->_response.autoIndex(ressource.root(), ressource.path(), ressource.responseCode());
		break;
	case RESSOURCE_ERROR:
		if (ressource.path().empty()) {
			this->_response.generate(ressource.responseCode());
		} else {
			this->_response.file(ressource.fullPath(), ressource.responseCode(), ressource.mimeType());
		}
		break;
	default:
		_exit(1);
	}
}

void HttpTransaction::handleErrorRessource(const Ressource &errorRessource, const HttpError &ressourceError) {
	if (errorRessource.path().empty()) {
		this->_response.generate(errorRessource.responseCode(), ressourceError.message());
	} else {
		this->_response.file(errorRessource.path(), errorRessource.responseCode(), errorRessource.mimeType());
	}
}

bool HttpTransaction::recvResponse(std::istream &input) {
	try {
		bool result = this->_response.recvFrom(input, this->nearestConfig());
		if (result) {
			return true;
		} else {
			if (this->_response.isWaitingRouting()) {
				this->_response.completeRouting();
				this->_response.recvFrom(input, this->nearestConfig());
			}
			return false;
		}
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
	const Ressource errorRessource(this->_request, e.status(), this->_serverConfig);
	this->handleErrorRessource(errorRessource, e);
	this->_response.keepAlive(false);
}

const Location &HttpTransaction::nearestConfig() const {
	return this->_resolvedConfig ? *this->_resolvedConfig : this->_serverConfig;
}

const struct sockaddr_storage &HttpTransaction::serverAddress() const {
	return this->_serverAddress;
}

void HttpTransaction::formatServerAddress(FormattedAddress &target) const {
	formatAddress(this->_serverAddress, target);
}

const struct sockaddr_storage &HttpTransaction::clientAddress() const {
	return this->_clientAddress;
}

void HttpTransaction::formatClientAddress(FormattedAddress &target) const {
	formatAddress(this->_clientAddress, target);
}

bool HttpTransaction::keepAlive() const {
	return this->_request.keepAlive() && this->_response.keepAlive();
}

void HttpTransaction::kill() {
	this->_response.keepAlive(false);
}
