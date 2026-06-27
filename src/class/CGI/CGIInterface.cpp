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
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

CGIInterface::CGIInterface(const std::string &execPath, HttpTransaction &httpTransaction, WebServer &server)
	: _execPath(execPath), _httpTransaction(httpTransaction), _inPipe(Pipe::createCGIPipe(*this)), _outPipe(Pipe::createCGIPipe(*this)) {
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

	if (httpTransaction.request().hasBody()) {
		server.epoll().add(out);
	} else {
		this->_outPipe.releaseIn();
	}

	server.epoll().add(in);

	int status = 0;

	pid_t pid = waitpid(this->_cgiPid, &status, WNOHANG);
	if (pid == -1) throw WebservErrors::SysError("waitpid", errno);
	if (pid == 0) return;
	this->_cgiPid = -1;
	std::cout << WEXITSTATUS(status) << std::endl;
}

void CGIInterface::startCgi(const HttpRequest &request) {
	this->_outPipe.releaseIn();
	this->_inPipe.releaseOut();

	AFd &childStdIn = this->_outPipe.out();
	AFd &childStdOut = this->_inPipe.in();

	if (childStdIn.dup2(0) == -1) {
		perror("dup2");
		_exit(1);
	}

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

	execve(this->_execPath.c_str(), argv, envp);
	perror("execve");
	_exit(1);
}

void CGIInterface::setupEnv(std::vector<std::string> &env, const HttpRequest &request) {
	const HeaderMap &headers = request.headers();

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
	env.push_back("PATH_INFO=/path/info");

	// TODO
	env.push_back("REMOTE_ADDR=127.0.0.1");
	// TODO
	env.push_back("REMOTE_HOST=127.0.0.1");
	env.push_back("REQUEST_METHOD=" + request.methodStr());
	// TODO
	env.push_back("QUERY_STRING=?qtest1=123&qtest2=abc");

	// TODO
	env.push_back("SERVER_NAME=webserv");
	// TODO
	env.push_back("SERVER_PORT=6969");
	env.push_back("SERVER_PROTOCOL=" + request.versionStr());
	env.push_back("SERVER_SOFTWARE=webserv/0.1");
}

void CGIInterface::inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		this->_httpTransaction.sendRequestBody(pipeIn);
	} else {
		this->_outPipe.releaseIn();
	}
}

void CGIInterface::outPipeEvent(const Pipe::Out &pipeOut, uint32_t events, WebServer &server) {
	(void)server;
	if (events & EPOLLIN) {
		char buffer[4096];
		std::stringstream input;

		ssize_t readLen = pipeOut.read(buffer, 4096);
		if (!readLen) this->_inPipe.releaseOut();
		if (readLen < 0) {
			this->_inPipe.releaseOut();
			throw WebservErrors::SysError("read", errno);
		}

		// std::cerr.write(buffer, readLen);
		input.write(buffer, readLen);

		if (this->_httpTransaction.recvResponse(input)) {

		} else {
			std::cerr << "WAITING " << this->_httpTransaction.response().status() << std::endl;
		}
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
