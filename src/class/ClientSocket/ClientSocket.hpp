#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include "ASocket/ASocket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class WebServer;
class ListeningSocket;

class ClientSocket : public ASocket {
  private:
	struct sockaddr_storage _address;
	socklen_t _addressLen;
	bool _closed;

	ClientSocket(int fd, struct sockaddr_storage &_address, socklen_t _addressLen);
	void onWriteReady();
	void onEpollIn();
	void onEpollOut();

	int readCount; // !:! temp
	bool responseSent; // !:! temp

  public:
	~ClientSocket();

	const struct sockaddr_storage &getAdress() const;
	socklen_t getAdressLen() const;

	u_int32_t getHandledEvents() const;
	void handleEvents(u_int32_t events, WebServer &webServer);

	bool closed() const { return _closed; }

	static ClientSocket *createFromListener(int listenerFd);
};

#endif
