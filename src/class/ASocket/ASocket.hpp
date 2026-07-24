#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "EpollInstance/EpollWatchable.hpp"
#include <sys/socket.h>

class ListeningSocket;

class ASocket : public AEpollWatchable {
  private:
	static int createFdFromListener(const ListeningSocket &listeningSocket, sockaddr_storage &outAddress);

  protected:
	ASocket(int fd, const sockaddr &address, socklen_t addressLen);
	ASocket(int fd, const sockaddr_storage &address, socklen_t addressLen);
	ASocket(const ListeningSocket &listeningSocket);

	sockaddr_storage _address;

  public:
	virtual ~ASocket();
};

#endif
