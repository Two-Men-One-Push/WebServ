#include "./CGIInterface.hpp"
#include "Pipe/Pipe.hpp"
#include "Ressource/Ressource.hpp"
#include "WebServer/WebServer.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpTransaction.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include "http/types.hpp"
#include "utils/formatting.hpp"
#include <cctype>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

CGIInterface::CGIInterface(const Ressource &ressource, HttpTransaction &httpTransaction, WebServer &server)
	: _httpTransaction(httpTransaction),
	  _inPipe(Pipe::createCGIPipe(*this)),
	  _outPipe(Pipe::createCGIPipe(*this)),
	  _processSucces(false) {
	this->_cgiPid = fork();

	if (this->_cgiPid == -1) throw WebservErrors::SysError("fork", errno);

	if (this->_cgiPid) {
		this->startInterface(httpTransaction, server);
	} else {
		this->startCgi(httpTransaction, ressource);
	}
}

CGIInterface::~CGIInterface() {
	this->killChild();
}

std::string CGIInterface::toEnvCase(const std::string &s) {
	std::string result;
	for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
		if (*it == '-') result += '_';
		else result += ::toupper(static_cast<unsigned char>(*it));
	}
	return result;
}

void CGIInterface::startInterface(HttpTransaction &httpTransaction, WebServer &server) {
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
}

void CGIInterface::startCgi(const HttpTransaction &httpTransaction, const Ressource &ressource) {
	this->_outPipe.releaseIn();
	this->_inPipe.releaseOut();

	Fd &childStdIn = this->_outPipe.out();
	Fd &childStdOut = this->_inPipe.in();

	if (childStdIn.dup2(0) == -1) {
		perror("dup2");
		_exit(1);
	}

	if (childStdOut.dup2(1) == -1) {
		perror("dup2");
		_exit(1);
	}

	const std::string &scriptPath = ressource.fullPath();
	char *const argv[] = {const_cast<char *>(ressource.cgiInterpreter().c_str()), const_cast<char *>(scriptPath.c_str()), NULL};

	std::vector<std::string> env;
	this->setupEnv(env, httpTransaction, ressource);
	char **envp = new char *[env.size() + 1];

	size_t i = 0;
	while (i < env.size()) {
		envp[i] = const_cast<char *>(env[i].c_str());
		++i;
	}
	envp[i] = NULL;

	execve(ressource.cgiInterpreter().c_str(), argv, envp);
	perror("execve");
	_exit(1);
}

void CGIInterface::setupEnv(std::vector<std::string> &env, const HttpTransaction &httpTransaction, const Ressource &ressource) {
	const HttpRequest &request = httpTransaction.request();
	std::ostringstream ss;
	FormattedAddress formattedAddress;

	if (request.hasBody()) {
		ss << request.contentLength();
		env.push_back("CONTENT_LENGTH=" + ss.str());
		ss.str("");
		env.push_back("CONTENT_TYPE=" + request.mimeType());
	}

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");

	env.push_back("SCRIPT_NAME=" + ressource.scriptName());
	env.push_back("PATH_TRANSLATED=" + ressource.path());
	env.push_back("PATH_INFO=" + ressource.pathInfo());

	env.push_back("REQUEST_METHOD=" + request.methodStr());
	env.push_back("QUERY_STRING=" + request.uri().queryString());

	env.push_back("SERVER_NAME=" + request.host().first);

	httpTransaction.formatClientAddress(formattedAddress);
	env.push_back("REMOTE_ADDR=" + formattedAddress.address);

	httpTransaction.formatServerAddress(formattedAddress);
	ss << formattedAddress.port;
	env.push_back("SERVER_PORT=" + ss.str());
	ss.str("");
	env.push_back("SERVER_PROTOCOL=" + request.versionStr());
	env.push_back("SERVER_SOFTWARE=webserv/0.1");

	for (HeaderMap::const_iterator it = request.headers().begin(); it != request.headers().end(); ++it) {
		if (it->first == "Content-Length" || it->first == "Content-Type") continue;
		env.push_back("HTTP_" + CGIInterface::toEnvCase(it->first) + '=' + it->second);
	}
}

void CGIInterface::inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer) {
	(void)webServer;
	if (events & EPOLLOUT) {
		if (this->_httpTransaction.sendRequestBody(pipeIn)) {
			this->_outPipe.releaseIn();
		}
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

		// std::cerr << "\e[0;31m";
		// std::cerr.write(buffer, readLen);
		// std::cerr << "\e[0m\n";
		input.write(buffer, readLen);

		if (this->_httpTransaction.recvResponse(input)) {
			this->_inPipe.releaseOut();
			int childResult = this->waitChild();
			if (childResult < 0) return;
			else if (childResult) {
				this->_httpTransaction.closeResponseInput();
			} else {
				this->_httpTransaction.error(HttpErrors::InternalServerErrorException());
			}
		} else {
			return;
		}
	} else {
		this->_inPipe.releaseOut();
		if (this->killChild()) {
			this->_httpTransaction.closeResponseInput();
		} else {
			this->_httpTransaction.error(HttpErrors::InternalServerErrorException());
		}
	}
}

bool CGIInterface::killChild() {
	if (this->_cgiPid > 0) {
		kill(this->_cgiPid, SIGKILL);
		int status;
		waitpid(this->_cgiPid, &status, 0);
		this->_processSucces = WIFEXITED(status) && (WEXITSTATUS(status) == 0);
		this->_cgiPid = -1;
	}
	return this->_processSucces;
}

int CGIInterface::waitChild() {
	if (this->_cgiPid > 0) {
		int status;
		if (!waitpid(this->_cgiPid, &status, WNOHANG)) {
			return -1;
		}
		this->_processSucces = WIFEXITED(status) && (WEXITSTATUS(status) == 0);
		this->_cgiPid = -1;
	}
	return this->_processSucces;
}

bool CGIInterface::running() const {
	return this->_cgiPid > 0;
}

Pipe &CGIInterface::in() {
	return this->_inPipe;
}

Pipe &CGIInterface::out() {
	return this->_outPipe;
}
