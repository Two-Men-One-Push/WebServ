#include "./HttpRequest.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include <algorithm>

void HttpRequest::loadHost() {
	HeaderMap &headers = this->_headers;
	if (!headers.has("Host")) throw HttpErrors::BadRequestException("CACA");
	const std::string &host = headers.at("Host");
	if (host.empty()) throw HttpErrors::BadRequestException();

	size_t bracketPos = host.find_last_of(']');
	size_t colonPos = host.find_last_of(':');
	if (colonPos == std::string::npos || (bracketPos != std::string::npos && bracketPos > colonPos)) {
		this->_host.first = host;
		this->_host.second = "";
	} else {
		this->_host.first = host.substr(0, colonPos);
		this->_host.second = host.substr(colonPos + 1);
		if (!this->_host.second.empty() &&
			this->_host.second.find_first_not_of("0123456789") != std::string::npos) {
			throw HttpErrors::BadRequestException();
		}
	}
	std::transform(this->_host.first.begin(), this->_host.first.end(), this->_host.first.begin(), ::tolower);
}
