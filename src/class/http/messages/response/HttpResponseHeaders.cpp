#include "./HttpResponse.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/types.hpp"
#include "utils/parsing.hpp"

void HttpResponse::loadCGIStatus() {
	HeaderMap &headers = this->_headers;
	if (!headers.has("Status")) return;
	const std::string &statusString = headers.at("Status");
	size_t spacePos = statusString.find(' ');
	if (spacePos == statusString.npos) {
		throw HttpErrors::InternalServerErrorException();
	}
	try {
		this->_status = HttpStatus::fromInt(parseULong(statusString.substr(0, spacePos)));
	} catch (...) {
		throw HttpErrors::InternalServerErrorException();
	}
	this->_message = statusString.substr(spacePos + 1);
	headers.erase("Status");
}

void HttpResponse::loadContentType() {
	HeaderMap &headers = this->_headers;
	if (!headers.has("Content-Type")) return;

	const std::string &statusString = headers.at("Status");
	size_t spacePos = statusString.find(' ');
	if (spacePos == statusString.npos) {
		throw HttpErrors::InternalServerErrorException();
	}
	try {
		this->_status = HttpStatus::fromInt(parseULong(statusString.substr(0, spacePos)));
	} catch (...) {
		throw HttpErrors::InternalServerErrorException();
	}
	this->_message = statusString.substr(spacePos + 1);
	headers.erase("Status");
}
