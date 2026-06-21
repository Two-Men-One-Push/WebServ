#ifndef CGIHPP
#define CGIHPP

#include "Pipe/Pipe.hpp"
#include "http/messages/request/HttpRequest.hpp"
#include <string>


class CGIInterface: public Pipe::IPipeWriter, public Pipe::IPipeReader {
  private:
	CGIInterface(const CGIInterface &other);

	std::string _execPath;

	Pipe _inPipe;
	Pipe _outPipe;

  public:
	CGIInterface(const std::string &execPath, HttpRequest &request);
	~CGIInterface();

	const std::string &execPath() const;

	const Pipe &in() const;
	const Pipe &out() const;
};

#endif
