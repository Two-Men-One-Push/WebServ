#ifndef HTTPTYPED_HPP
#define HTTPTYPED_HPP

#include <map>
#include <string>

class Headers : public std::map<std::string, std::string> {
  public:
	bool has(std::string key) const { return this->find(key) != this->end(); }
};

enum HttpVersion {
	HTTP1_0,
	HTTP1_1,
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

enum TransferEncoding {
	TE_UNDEFINED,
	TE_IDENTITY,
	TE_CHUNKED,
};

std::string httpVersionString(HttpVersion version);
std::string httpMethodString(HttpMethod method);
std::string transferEncodingString(TransferEncoding tranfertEncoding);

#endif
