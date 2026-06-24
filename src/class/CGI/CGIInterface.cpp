#include "./CGIInterface.hpp"
#include "Pipe/Pipe.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/types.hpp"
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

CGIInterface::CGIInterface(const std::string &execPath, HttpTransaction &httpTransaction, WebServer &server) : _execPath(execPath), _inPipe(Pipe::createCGIPipe(*this)), _outPipe(Pipe::createCGIPipe(*this)) {
	errno = 0;
	int pid = fork();

	if (pid == -1) throw WebservErrors::SysError("fork", errno);

	if (pid) {
		this->_cgiPid = pid;
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

	Pipe::In &out = this->_outPipe.in();
	Pipe::Out &in = this->_inPipe.out();

	server.epoll().add(out);
	server.epoll().add(in);

	int status = 0;

	errno = 0;
	pid_t pid = waitpid(this->_cgiPid, &status, WNOHANG);
	if (pid == -1) throw WebservErrors::SysError("waitpid", errno);
	if (pid == 0) return;
	this->_cgiPid = -1;
	std::cout << WEXITSTATUS(status) << std::endl;
}

void CGIInterface::startCgi(HttpRequest &request) {
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

	std::vector<std::string> env;
	this->setupEnv(env, request);
	char **envp = new char *[env.size() + 1];

	size_t i = 0;
	while (i < env.size()) {
		envp[i] = (char *)(env[i].c_str());
		++i;
	}
	envp[i] = NULL;

	errno = 0;
	execve(this->_execPath.c_str(), argv, envp);
	perror("execve");
	_exit(1);
}
void CGIInterface::setupEnv(std::vector<std::string> &env, HttpRequest &request) {
	HeaderMap &headers = request.headers();

	env.push_back("AUTH_TYPE=");
	env.push_back("REMOTE_IDENT=");
	env.push_back("REMOTE_USER=");

	std::ostringstream formatter;
	formatter << "CONTENT_LENGTH=" << request.contentLength();
	env.push_back(formatter.str());
	formatter.str("");

	if (request.hasBody() && headers.has("Content-Type")) {
		env.push_back("CONTENT_TYPE=" + headers.at("Content-Type"));
	} else {
		env.push_back("CONTENT_TYPE=");
	}

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	env.push_back("SCRIPT_NAME=" + this->_execPath);
	// TODO
	env.push_back("PATH_INFO=");

	// TODO
	env.push_back("REMOTE_ADDR=");
	// TODO
	env.push_back("REMOTE_HOST=");
	env.push_back("REQUEST_METHOD=" + request.methodStr());
	// TODO
	env.push_back("QUERY_STRING=");

	// TODO
	env.push_back("SERVER_NAME=");
	// TODO
	env.push_back("SERVER_PORT=");
	env.push_back("SERVER_PROTOCOL=" + request.versionStr());
	env.push_back("SERVER_SOFTWARE=webserv/0.1");
}

void CGIInterface::inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		std::string data("123456789123456789123456789");
		pipeIn.write(data.c_str(), data.size());
		this->_outPipe.releaseIn();
	} else {
		this->_outPipe.releaseIn();
	}
}

void CGIInterface::outPipeEvent(const Pipe::Out &pipeOut, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLIN) {
		char buffer[8];
		ssize_t size = pipeOut.read(buffer, 8);
		std::string input(buffer, size);
		std::cout << input << std::endl;
	} else {
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
