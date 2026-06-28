#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class ClientSocket;

class ListeningSocket : public ASocket {
  private:
	ListeningSocket(int socketFd, const sockaddr &address, socklen_t addressLen);
	void onEpollIn(WebServer &webServer) const;

  public:
	~ListeningSocket();

	ClientSocket *acceptConnexion(void) const;

	void handleEvents(u_int32_t events, WebServer &webServer);
	u_int32_t getHandledEvents() const;

	static ListeningSocket create(const sockaddr &addr, socklen_t addresslen);
	static ListeningSocket *createNew(const sockaddr &addr, socklen_t addresslen);
};

#endif
