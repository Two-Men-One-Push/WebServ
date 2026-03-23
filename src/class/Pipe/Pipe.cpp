#include "Pipe.hpp"
#include "errors/WebservErrors.hpp"
#include <netinet/in.h>
#include <unistd.h>

Pipe::Pipe(int fdIn, int fdOut) : _in(fdIn), _out(fdOut) {}

const Pipe::PipeIn &Pipe::in() const {
	return _in;
}

const Pipe::PipeOut &Pipe::out() const {
	return _out;
}

Pipe Pipe::createPipe() {
	int pipeFds[2];

	if (pipe(pipeFds) < 0) throw webserv_errors::SysError("pipe", errno);
	return Pipe(pipeFds[0], pipeFds[1]);
}
