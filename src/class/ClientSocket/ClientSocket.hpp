#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include "http/HttpTransaction.hpp"
#include "model/Server/Server.hpp"
#include <netinet/in.h>
#include <queue>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>

class WebServer;
class ListeningSocket;

class ClientSocket : public ASocket {
  private:
	const Server &_serverConfig;
	struct sockaddr_storage _address;
	socklen_t _addressLen;
	bool _inClosed;

	ClientSocket(int fd, struct sockaddr_storage &_address, socklen_t _addressLen, const Server &serverConfig);
	void onWriteReady();
	void onEpollIn(WebServer &webServer);
	void onEpollOut(WebServer &webServer);

	std::stringstream _outBuffer;
	std::queue<HttpTransaction *> _transactions;

	bool canHandleEpollOut() const;

  public:
	~ClientSocket();

	const struct sockaddr_storage &address() const;
	socklen_t addressLen() const;

	u_int32_t getHandledEvents() const;
	void handleEvents(u_int32_t events, WebServer &webServer);

	bool closed() const { return _inClosed; }

	int fd() const { return _fd; }

	static ClientSocket *createFromListener(int listenerFd, const Server &serverConfig);
};

#endif
