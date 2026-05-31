#include "http/messages/HttpRequest.hpp"
#include "http/HttpConnection.hpp"
#include "http/messages/HttpMessage.hpp"
#include "http/types.hpp"
#include <cctype>
#include <map>
#include <string>
#include <utility>

// cleanest way to define initialised map in cpp98

static std::pair<std::string, HttpMethod> pairs[] = {
	std::make_pair("GET", GET),
	std::make_pair("HEAD", HEAD),
	std::make_pair("DELETE", DELETE),
};

static int httpMethodCount = sizeof(pairs) / sizeof(pairs[0]);

std::map<std::string, HttpMethod> HttpRequest::implementedHttpMethod(pairs, pairs + httpMethodCount);

HttpRequest::HttpRequest(HttpConnection &connnection) : HttpMessage(connnection),
														_method(UNKNOWN),
														_uri() {}

HttpRequest::HttpRequest(const HttpRequest &other, HttpConnection &connnection) : HttpMessage(other, connnection),
																				  _method(),
																				  _uri() {}

HttpRequest::~HttpRequest() {}

std::string HttpRequest::getAllowHeader() {
	std::string allowHeader;

	for (std::map<std::string, HttpMethod>::const_iterator it = HttpRequest::implementedHttpMethod.begin(); it != HttpRequest::implementedHttpMethod.end(); ++it) {
		if (!allowHeader.empty())
			allowHeader += ", ";
		allowHeader += it->first;
	}
	return allowHeader;
}

HttpMethod HttpRequest::method() const {
	return this->_method;
}

std::string HttpRequest::methodStr() const {
	return httpMethodString(this->_method);
}

const std::string &HttpRequest::uri() const {
	return this->_uri;
}
