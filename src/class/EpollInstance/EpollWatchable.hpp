#ifndef AEPOLLWATCHABLEHPP
#define AEPOLLWATCHABLEHPP

#include "EpollInstance/EpollInstance.hpp"
#include <stdint.h>

class AEpollWatchable : public Fd { // contrat epoll, indépendant de la possession du fd
  private:
	const EpollInstance *_epoll;

  public:
	AEpollWatchable(int fd);
	virtual ~AEpollWatchable();

	virtual uint32_t getHandledEvents() const = 0;
	virtual void handleEvents(uint32_t events, WebServer &server) = 0;

	void epoll(const EpollInstance *epoll);
	const EpollInstance *epoll() const;
};

#endif
