#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "defaultErrorPage.hpp"

void HttpResponse::generate(HttpStatus::Code status) {
	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	printDefaultErrorPage(*ssBody, status);

	this->_bodyType = BT_CONTENT_LENGTH;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "text/html";
	this->_contentLength = ssBody->str().size();
	this->_inState = RECV_COMPLETED;
}

void HttpResponse::generate(HttpStatus::Code status, const std::string &message) {
	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	printDefaultErrorPage(*ssBody, status, message);

	this->_bodyType = BT_CONTENT_LENGTH;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "text/html";
	this->_contentLength = ssBody->str().size();
	this->_inState = RECV_COMPLETED;
}
