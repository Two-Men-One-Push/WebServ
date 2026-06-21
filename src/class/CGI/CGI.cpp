#include "./CGI.hpp"
#include "Pipe/Pipe.hpp"
#include "http/messages/request/HttpRequest.hpp"

CGIInterface::CGIInterface(const std::string &execPath, HttpRequest &request) : _execPath(execPath), _inPipe(Pipe::createCGIPipe(*this)), _outPipe(Pipe::createCGIPipe(*this)) {
	(void)request;
}

CGIInterface::~CGIInterface() {}

const std::string &CGIInterface::execPath() const {
	return this->_execPath;
}

const Pipe &CGIInterface::in() const {
	return this->_inPipe;
}

const Pipe &CGIInterface::out() const {
	return this->_outPipe;
}
