#ifndef CGI_HPP
#define CGI_HPP

#include "Pipe/Pipe.hpp"
#include "Ressource/Ressource.hpp"
#include <ctime>
#include <string>
#include <sys/types.h>

class HttpTransaction;
class WebServer;

class CGIInterface : public Pipe::IPipeWriter, public Pipe::IPipeReader {
  private:
	WebServer &_server;
	HttpTransaction &_httpTransaction;

	/* The Pipe the parent process reads from */
	Pipe _inPipe;

	/* The Pipe the parent process writes in */
	Pipe _outPipe;

	pid_t _cgiPid;

	bool _processSucces;

	std::time_t _lastActivity;

	void startInterface(HttpTransaction &httpTransaction, WebServer &server);
	void startCgi(const HttpTransaction &httpTransaction, const Ressource &ressource);
	void setupEnv(std::vector<std::string> &env, const HttpTransaction &httpTransaction, const Ressource &ressource);
	int waitChild();
	bool killChild();

  public:
	CGIInterface(const Ressource &ressource, HttpTransaction &httpTransaction, WebServer &server);
	virtual ~CGIInterface();

	/** Do you think this is related to _inPipe or _outPipe ? guess */
	void outPipeEvent(const Pipe::Out &pipeOut, uint32_t events, WebServer &webServer);

	/** Do you think this is related to _inPipe or _outPipe ? guess */
	void inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer);

	void checkTimeout();
	void requestDelete();
	bool running() const;

	Pipe &in();
	Pipe &out();

	static std::string toEnvCase(const std::string &s);
};

#endif
