#ifndef CGI_HPP
#define CGI_HPP

#include "Pipe/Pipe.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include <string>
#include <sys/types.h>

class HttpTransaction;
class WebServer;

class CGIInterface : public Pipe::IPipeWriter, public Pipe::IPipeReader {
  private:
	std::string _execPath;
	HttpTransaction &_httpTransaction;

	/* The Pipe the parent process reads from */
	Pipe _inPipe;

	/* The Pipe the parent process writes in */
	Pipe _outPipe;

	pid_t _cgiPid;

	bool _processSucces;

	void startInterface(HttpTransaction &httpTransaction, WebServer &server);
	void startCgi(const HttpRequest &request);
	void setupEnv(std::vector<std::string> &env, const HttpRequest &request);
	int waitChild();
	bool killChild();

  public:
	CGIInterface(const std::string &execPath, HttpTransaction &httpTransaction, WebServer &server);
	virtual ~CGIInterface();

	const std::string &execPath() const;

	/** Do you think this is related to _inPipe or _outPipe ? guess */
	void outPipeEvent(const Pipe::Out &pipeOut, uint32_t events, WebServer &webServer);

	/** Do you think this is related to _inPipe or _outPipe ? guess */
	void inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer);

	bool running() const;

	Pipe &in();
	Pipe &out();
};

#endif
