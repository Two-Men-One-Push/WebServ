#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/HttpMessage.hpp"

void HttpResponse::uploaded(const std::string &uploadPath) {
	this->replaceBody(NULL);

	this->_bodyType = BT_NONE;
	this->_status = HttpStatus::Created;
	this->_message = HttpStatus::reasonPhrase(HttpStatus::Created);
	this->_contentLength = 0;
	this->_location = uploadPath;
	this->_inState = RECV_COMPLETED;
}
