#include "utils/formatting.hpp"
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>

void formatAddress(struct sockaddr_storage &address, FormattedAddress &out) {
	std::stringstream ss;

	out.family = address.ss_family;
	if (address.ss_family == AF_INET) {
		struct sockaddr_in *inAddress = reinterpret_cast<sockaddr_in *>(&address);
		uint32_t intAddress = ntohl(inAddress->sin_addr.s_addr);

		ss << (intAddress >> 24) << '.'
		   << ((intAddress >> 16) & 0xFF) << '.'
		   << ((intAddress >> 8) & 0xFF) << '.'
		   << (intAddress & 0xFF);
		out.address = ss.str();
		out.port = ntohs(inAddress->sin_port);
	} else {
		struct sockaddr_in6 *in6Address = reinterpret_cast<sockaddr_in6 *>(&address);

		out.address = "[SPOO:KY S:CARY:IPV6]";
		out.port = ntohs(in6Address->sin6_port);
	}
}
