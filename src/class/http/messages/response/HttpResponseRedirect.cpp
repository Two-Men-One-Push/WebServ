#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "http/messages/HttpMessage.hpp"

void HttpResponse::redirect(const std::string &redirectUrl, HttpStatus::Code status) {
	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	*ssBody << status;

	this->_bodyType = BT_CONTENT_LENGTH;
	this->_redirectUrl = redirectUrl;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "";
	this->_contentLength = 0;
	this->_inState = RECV_COMPLETED;
}
