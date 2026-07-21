#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/HttpMessage.hpp"
#include "defaultErrorPage.hpp"

void HttpResponse::redirect(const std::string &redirectUrl, HttpStatus::Code status) {
	this->replaceBody(NULL);

	this->_bodyType = BT_NONE;
	this->_location = redirectUrl;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "";
	this->_contentLength = 0;
	this->_inState = RECV_COMPLETED;
}
