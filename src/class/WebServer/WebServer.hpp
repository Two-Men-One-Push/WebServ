#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Fd/Fd.hpp"
#include "ClientSocket/ClientSocket.hpp"
#include "EpollInstance/EpollInstance.hpp"
#include "ListeningSocket/ListeningSocket.hpp"
#include <vector>

class WebServer {
  private:
	EpollInstance _epoll;
	std::vector<ListeningSocket *> _listeningSockets;
	std::vector<ClientSocket *> _clientSockets;
	std::vector<ClientSocket *> _clientSocketsToDelete;

	void deleteClientSockets();

  public:
	WebServer();
	~WebServer();

	void addClient(ClientSocket *client);
	void requestDelete(ClientSocket *client);
	const EpollInstance &epoll() const;
};

#endif
