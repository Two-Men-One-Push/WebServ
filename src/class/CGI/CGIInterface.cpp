#include "./CGIInterface.hpp"
#include "Pipe/Pipe.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

CGIInterface::CGIInterface(const std::string &execPath, HttpTransaction &httpTransaction, WebServer &server) : _execPath(execPath), _inPipe(Pipe::createCGIPipe(*this)), _outPipe(Pipe::createCGIPipe(*this)) {
	errno = 0;
	int pid = fork();

	if (pid == -1) throw WebservErrors::SysError("fork", errno);

	this->_cgiPid = pid;
	if (pid) {
		this->startInterface(httpTransaction, server);
	} else {
		this->startCgi(httpTransaction.request());
	}
}

CGIInterface::~CGIInterface() {
	if (this->_cgiPid > 0) {
		kill(this->_cgiPid, SIGKILL);
		waitpid(this->_cgiPid, NULL, 0);
	}
}

void CGIInterface::startInterface(HttpTransaction &httpTransaction, WebServer &server) {
	(void)httpTransaction;

	this->_outPipe.releaseOut();
	this->_inPipe.releaseIn();

	AFd &out = this->_outPipe.in();
	AFd &in = this->_inPipe.out();

	server.epoll().registerFd(out);
	server.epoll().registerFd(in);

	int status = 0;

	errno = 0;
	pid_t pid = waitpid(this->_cgiPid, &status, WNOHANG);
	if (pid == -1) throw WebservErrors::SysError("waitpid", errno);
	if (pid == 0) return;
	this->_cgiPid = -1;
	std::cout << WEXITSTATUS(status) << std::endl;
}

void CGIInterface::startCgi(HttpRequest &request) {
	(void)request;

	this->_outPipe.releaseIn();
	this->_inPipe.releaseOut();

	AFd &childStdIn = this->_outPipe.out();
	AFd &childStdOut = this->_inPipe.in();

	errno = 0;
	if (childStdIn.dup2(0) == -1) {
		perror("dup2");
		_exit(1);
	}

	errno = 0;
	if (childStdOut.dup2(1) == -1) {
		perror("dup2");
		_exit(1);
	}

	char *const argv[] = {const_cast<char *>(this->_execPath.c_str()), NULL};
	errno = 0;
	execve(this->_execPath.c_str(), argv, environ);
	perror("execve");
	_exit(1);
}

void CGIInterface::inPipeEvent(const AFd &pipeIn, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		std::string data("abcde");
		pipeIn.write(data.c_str(), data.size());
		this->_outPipe.releaseIn();
	} else {
		if (events & EPOLLERR) {
			std::cout << "pipeIn EPOLLERR" << std::endl;
		}
		if (events & EPOLLHUP) {
			std::cout << "pipeIn EPOLLHUP" << std::endl;
		}
		this->_outPipe.releaseIn();
	}
}

void CGIInterface::outPipeEvent(const AFd &pipeOut, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLIN) {
		std::cout << "pipeOut EPOLLIN" << std::endl;
		char buffer[8];
		ssize_t size = pipeOut.read(buffer, 8);
		write(1, buffer, size);
	} else {
		if (events & EPOLLERR) {
			std::cout << "pipeOut EPOLLERR" << std::endl;
		}
		if (events & EPOLLHUP) {
			std::cout << "pipeOut EPOLLHUP" << std::endl;
		}
		this->_inPipe.releaseOut();
	}
}

const std::string &CGIInterface::execPath() const {
	return this->_execPath;
}

Pipe &CGIInterface::in() {
	return this->_inPipe;
}

Pipe &CGIInterface::out() {
	return this->_outPipe;
}
