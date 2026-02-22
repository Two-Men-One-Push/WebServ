#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class ClientSocket;

class ListeningSocket : ASocket {
  private:
	ListeningSocket(int socketFd, const struct sockaddr_storage &address);

  public:
	~ListeningSocket();

	ClientSocket *acceptConnexion(void) const;
	int getFd(void) const;

	static ListeningSocket create(const struct sockaddr_storage &address);
};

#endif
