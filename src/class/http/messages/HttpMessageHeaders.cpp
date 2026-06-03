#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"

void HttpMessage::loadTranferEncoding() {
	this->_transferEncoding = TE_UNDEFINED;
}
