#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "EpollInstance/EpollWatchable.hpp"

class ASocket : public AEpollWatchable {
  protected:
	ASocket(int fd);

  public:
	virtual ~ASocket();
};

#endif
