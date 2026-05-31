#ifndef HTTPTYPED_HPP
#define HTTPTYPED_HPP

#include <map>
#include <string>

typedef std::map<std::string, std::string> Headers;

enum HttpVersion {
	HTTP1_0,
	HTTP1_1,
};

enum TransferEncoding {
	TE_IDENTITY,
	TE_CHUNKED,
};

enum HttpMethod {
	UNKNOWN = 0,
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	TRACE,
	CONNECT,
};

std::string httpVersionString(HttpVersion version);
std::string httpMethodString(HttpMethod method);

#endif
