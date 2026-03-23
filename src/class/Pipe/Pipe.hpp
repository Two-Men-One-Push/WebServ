#ifndef PIPE_HPP
#define PIPE_HPP

#include "WebServer/WebServer.hpp"
#include <fcntl.h>
#include <stdint.h>

class Pipe {
  private:
	class PipeIn : public AFd {
	  public:
		PipeIn(int fd);
		~PipeIn();

		uint32_t getHandledEvents() const; // Subject to change
		void handleEvents(uint32_t events, WebServer &webServer); // Subject to change
	};

	class PipeOut : public AFd {
	  public:
		PipeOut(int fd);
		~PipeOut();

		uint32_t getHandledEvents() const; // Subject to change
		void handleEvents(uint32_t events, WebServer &webServer); // Subject to change
	};

	PipeIn _in;
	PipeOut _out;

	Pipe(int fdIn, int fdOut);

  public:
	~Pipe();

	const PipeIn &in() const;
	const PipeOut &out() const;

	static Pipe createPipe();
};

#endif
