#include "http/messages/response/HttpResponse.hpp"
#include <cerrno>
#include <sstream>
#include <sys/types.h>

void HttpResponse::formatTypeLine() {
	std::stringstream ss;

	ss << this->versionStr() << ' ' << this->_status << ' ' << this->_message << "\r\n";

	this->_outBuffer += ss.str();
}
