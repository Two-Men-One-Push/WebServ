#include "CGI/CGIInterface.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/messages/response/HttpResponse.hpp"

void HttpResponse::cgi(CGIInterface &cgiInterface) {
	this->_status = HttpStatus::OK;
	this->_message = HttpStatus::reasonPhrase(HttpStatus::OK);
	this->_inState = RECV_MESSAGE_HEADERS;
	this->_body = new BodyStringStream();
	this->_cgiInterface = &cgiInterface;
}
