#include "EpollInstance/EpollWatchable.hpp"
#include "Fd/Fd.hpp"
#include "EpollInstance/EpollInstance.hpp"

AEpollWatchable::AEpollWatchable(int fd) : Fd(fd), _epoll(NULL) {}

AEpollWatchable::~AEpollWatchable() {
	if (this->_epoll != NULL)
		this->_epoll->del(this->fd());
}

void AEpollWatchable::epoll(const EpollInstance *epoll) {
	this->_epoll = epoll;
}

const EpollInstance *AEpollWatchable::epoll() const {
	return this->_epoll;
}
