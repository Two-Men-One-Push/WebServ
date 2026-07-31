#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <stdint.h>
#include <string>
#include <sys/socket.h>

struct FormattedAddress {
	sa_family_t family;
	std::string address;
	uint16_t port;
};

void formatAddress(const struct sockaddr_storage &address, FormattedAddress &out);

void printEscapedHtml(std::ostream &os, const std::string &htmlText);
std::string formatSize(std::size_t n);

#endif
