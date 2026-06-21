#ifndef PIPE_HPP
#define PIPE_HPP

#include "WebServer/WebServer.hpp"
#include <fcntl.h>
#include <stdint.h>

class CGIInterface;

class Pipe {
  public:
	class IPipeWriter {
	  public:
		virtual void onPipeIn(const AFd &pipeIn) = 0;
	};

	class IPipeReader {
	  public:
		virtual void onPipeOut(const AFd &pipeOut) = 0;
	};

  private:
	class In : public AFd {
	  private:
		IPipeWriter &_target;

	  public:
		In(int fd, IPipeWriter &target);
		~In();

		uint32_t getHandledEvents() const;
		void handleEvents(uint32_t events, WebServer &webServer);
	};

	class Out : public AFd {
	  private:
		IPipeReader &_target;

	  public:
		Out(int fd, IPipeReader &target);
		~Out();

		uint32_t getHandledEvents() const;
		void handleEvents(uint32_t events, WebServer &webServer);
	};

	In *_in;
	Out *_out;

	Pipe(int fdIn, int fdOut, Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget);

  public:
	~Pipe();

	const In &in() const;
	const Out &out() const;

	void releaseIn();
	void releaseOut();

	static Pipe createPipe(Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget);
	static Pipe createCGIPipe(CGIInterface &cgi);
};

#endif
