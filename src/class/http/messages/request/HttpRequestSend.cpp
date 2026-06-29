#include "./HttpRequest.hpp"
#include <sstream>

void HttpRequest::formatTypeLine() {
	std::stringstream ss;

	ss << this->methodStr() << ' ' << this->_uri << ' ' << this->versionStr() << "\r\n";

	this->_outBuffer += ss.str();
}
