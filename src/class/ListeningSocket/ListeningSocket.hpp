#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include "model/Server/Server.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class ClientSocket;

class ListeningSocket : public ASocket {
  private:
	const Server &_serverConfig;
	void onEpollIn(WebServer &webServer) const;
	void onEpollErr(WebServer &webServer) const;

	static int createFd(const sockaddr &addr);

  public:
	ListeningSocket(const sockaddr &address, socklen_t addresslen, const Server &serverConfig);
	~ListeningSocket();

	ClientSocket *acceptConnexion(void) const;

	void handleEvents(u_int32_t events, WebServer &webServer);
	u_int32_t getHandledEvents() const;

	int fd() const { return _fd; }

	const Server &serverConfig() const { return _serverConfig; };

	const sockaddr_storage &address() const { return this->_address; };

	int accept(struct sockaddr *address, socklen_t *addressLen) const;

	static ListeningSocket create(const sockaddr &addr, socklen_t addresslen, const Server &serverConfig);
	static ListeningSocket *createNew(const sockaddr &addr, socklen_t addresslen, const Server &serverConfig);
};

#endif
