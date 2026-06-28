#include "CGI/CGIInterface.hpp"
#include "http/messages/response/HttpResponse.hpp"
#include <sstream>


void HttpResponse::cgi(CGIInterface &cgiInterface) {
	this->_body = new std::stringstream();
	this->_cgiInterface = &cgiInterface;
}
