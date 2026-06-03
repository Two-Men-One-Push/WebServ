#include "http/types.hpp"
#include <string>

std::string httpVersionString(HttpVersion version) {
	if (version == HTTP1_0) return "HTTP/1.0";
	if (version == HTTP1_1) return "HTTP/1.1";
	return "UNKNOWN";
}

std::string httpMethodString(HttpMethod method) {
	if (method == GET) return "GET";
	if (method == HEAD) return "HEAD";
	if (method == POST) return "POST";
	if (method == PUT) return "PUT";
	if (method == DELETE) return "DELETE";
	if (method == TRACE) return "TRACE";
	if (method == CONNECT) return "CONNECT";
	return "UNKNOWN";
}

std::string transferEncodingString(TransferEncoding tranfertEncoding) {
	if (tranfertEncoding == TE_UNDEFINED) return "";
	if (tranfertEncoding == TE_CHUNKED) return "chunked";
	return "UNKNOWN";
}
