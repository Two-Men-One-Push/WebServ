#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/messages/Body/BodyStringStream.hpp"

void HttpResponse::autoIndex(const std::string &directoryPath, HttpStatus::Code status) {
	(void)directoryPath;

	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	*ssBody << status;

	this->_bodyType = BT_CONTENT_LENGTH;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "text/html";
	this->_contentLength = ssBody->str().size();
	this->_inState = RECV_COMPLETED;
}
