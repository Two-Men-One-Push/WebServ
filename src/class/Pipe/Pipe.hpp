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
		virtual void inPipeEvent(const AFd &pipeIn, uint32_t events, WebServer &webServer) = 0;
	};

	class IPipeReader {
	  public:
		virtual void outPipeEvent(const AFd &pipeOut, uint32_t events, WebServer &webServer) = 0;
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

	/** The AFd you read from */
	Out *_out;

	/** The AFd you write in */
	In *_in;

	Pipe(int fdOut, int fdIn, Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget);

  public:
	~Pipe();

	/** The AFd you write in */
	In &in();

	/** The AFd you read from */
	Out &out();

	/** Delete the the AFd you write in */
	void releaseIn();

	/** Delete the the AFd you read from */
	void releaseOut();

	static Pipe createPipe(Pipe::IPipeWriter &writerTarget, Pipe::IPipeReader &readerTarget);
	static Pipe createCGIPipe(CGIInterface &cgi);
};

#endif
