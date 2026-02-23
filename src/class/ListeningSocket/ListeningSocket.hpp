#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

class ClientSocket;

class ListeningSocket : ASocket {
  private:
	ListeningSocket(int socketFd, const sockaddr &address, socklen_t addressLen);

  public:
	~ListeningSocket();

	ClientSocket *acceptConnexion(void) const;
	int getFd(void) const;

	static ListeningSocket create(const sockaddr &addr, socklen_t addresslen);
};

#endif
