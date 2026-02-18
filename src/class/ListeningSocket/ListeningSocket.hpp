#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>

class ListeningSocket : ASocket {
  private:
	ListeningSocket(const struct sockaddr_storage &address);

  public:
	~ListeningSocket();

	ClientSocket *acceptConnexion(void) const;
	int getFd(void) const;

	static ListeningSocket createListeningSocket(const struct sockaddr_storage &address);
};

#endif
