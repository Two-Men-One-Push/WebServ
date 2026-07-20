#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "defaultErrorPage.hpp"

void HttpResponse::generate(HttpStatus::Code status) {
	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	generateErrorPage(*ssBody, status);

	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "text/html";
	this->_contentLength = ssBody->str().size();
	this->_inState = RECV_COMPLETED;
}
