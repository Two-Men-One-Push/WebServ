#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "EpollInstance/EpollWatchable.hpp"
#include <sys/socket.h>

class ASocket : public AEpollWatchable {
  protected:
	ASocket(int fd, const sockaddr &address, socklen_t addressLen);
	ASocket(int fd, const sockaddr_storage &address, socklen_t addressLen);
  	sockaddr_storage _address;

  public:
	virtual ~ASocket();
};

#endif
