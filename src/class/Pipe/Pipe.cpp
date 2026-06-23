#include "Pipe.hpp"
#include "CGI/CGIInterface.hpp"
#include "errors/WebservErrors.hpp"
#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

Pipe::Pipe(int fdOut, int fdIn, Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget)
	: _out(new Out(fdOut, readerTarget)), _in(new In(fdIn, writerTarget)) {}

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

Pipe::In &Pipe::in() {
	return *_in;
}

Pipe::Out &Pipe::out() {
	return *_out;
}

Pipe Pipe::createPipe(Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget) {
	int pipeFds[2];


	if (pipe(pipeFds) < 0) throw WebservErrors::SysError("pipe", errno);
	std::cout << pipeFds[0] << ":" << pipeFds[1] << std::endl;
	return Pipe(pipeFds[0], pipeFds[1], writerTarget, readerTarget);
}

Pipe Pipe::createCGIPipe(CGIInterface &cgi) {
	int pipeFds[2];


	if (pipe(pipeFds) < 0) throw WebservErrors::SysError("pipe", errno);
	std::cout << pipeFds[0] << ":" << pipeFds[1] << std::endl;
	return Pipe(pipeFds[0], pipeFds[1], cgi, cgi);
}
