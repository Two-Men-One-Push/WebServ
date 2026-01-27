#include "ClientSocket.hpp"
#include <iostream>
#include <sys/socket.h>

ClientSocket::~ClientSocket()
{}

ClientSocket::ClientSocket(int fd, struct sockaddr_storage &address, socklen_t addressLen): ASocket(fd), _address(address), _addressLen(addressLen)
{
	std::cout << "[INFO] New Client Connected" << std::endl;
}
