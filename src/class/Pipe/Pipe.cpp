#include "CGI/CGI.hpp"
#include "Pipe.hpp"
#include "errors/WebservErrors.hpp"
#include <cstddef>
#include <netinet/in.h>
#include <unistd.h>

Pipe::Pipe(int fdIn, int fdOut, Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget) : _in(new In(fdIn, writerTarget)), _out(new Out(fdOut, readerTarget)) {}

Pipe::~Pipe() {
	if (this->_in != NULL) {
		delete this->_in;
	}

	if (this->_out != NULL) {
		delete this->_out;
	}
}

void Pipe::releaseIn() {
	delete this->_in;
	this->_in = NULL;
}

void Pipe::releaseOut() {
	delete this->_out;
	this->_out = NULL;
}

const Pipe::In &Pipe::in() const {
	return *_in;
}

const Pipe::Out &Pipe::out() const {
	return *_out;
}

Pipe Pipe::createPipe(Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget) {
	int pipeFds[2];

	if (pipe(pipeFds) < 0) throw WebservErrors::SysError("pipe", errno);
	return Pipe(pipeFds[0], pipeFds[1], writerTarget, readerTarget);
}

Pipe Pipe::createCGIPipe(CGIInterface &cgi) {
	int pipeFds[2];

	if (pipe(pipeFds) < 0) throw WebservErrors::SysError("pipe", errno);
	return Pipe(pipeFds[0], pipeFds[1], cgi, cgi);
}
