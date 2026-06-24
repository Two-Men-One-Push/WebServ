#ifndef CGIHPP
#define CGIHPP

#include "Pipe/Pipe.hpp"
#include "WebServer/WebServer.hpp"
#include "http/HttpTransaction.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include <string>
#include <sys/types.h>


class CGIInterface: public Pipe::IPipeWriter, public Pipe::IPipeReader {
  private:
	std::string _execPath;

	/* The Pipe the parent process reads from */
	Pipe _inPipe;

	/* The Pipe the parent process writes in */
	Pipe _outPipe;

	pid_t _cgiPid;

	void startInterface(HttpTransaction &httpTransaction, WebServer &server);
	void startCgi(HttpRequest &request);
	void setupEnv(std::vector<std::string> &env, HttpRequest &request);

  public:
	CGIInterface(const std::string &execPath, HttpTransaction &httpTransaction, WebServer &server);
	~CGIInterface();

	const std::string &execPath() const;


	void inPipeEvent(const Pipe::In &pipeIn, uint32_t events, WebServer &webServer);
	void outPipeEvent(const Pipe::Out &pipeOut, uint32_t events, WebServer &webServer);

	Pipe &in();
	Pipe &out();
};

#endif
