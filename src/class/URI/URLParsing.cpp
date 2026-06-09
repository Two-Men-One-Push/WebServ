#include "./URL.hpp"
#include "utils/parsing.hpp"
#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string>

/**
 * @brief Tells if a string respects the RFC format for URI schemes
 * @see https://datatracker.ietf.org/doc/html/rfc3986#section-3
 */
static bool isScheme(const std::string &input) {
	if (!std::isalpha(input[0])) return false;
	for (std::string::const_iterator it = input.begin() + 1; it != input.end(); ++it) {
		if (!(URL::isunreserved(*it))) return false;
	}
	return true;
}

void URL::parse(const std::string &url) {
	std::size_t found;
	std::size_t pos;

	found = url.find("://");
	if (found == std::string::npos) throw std::invalid_argument("\"" + url + "\" is not a valid URL");
	this->_protocol = url.substr(0, found);
	if (!isScheme(this->_protocol)) throw std::invalid_argument("\"" + url + "\" is not a valid URL");
	pos = found;
	found = url.find_first_of("/?#");
	std::string authority = url.substr(pos, found - pos);
	this->parseAuthority(authority, url);
}

void URL::parseAuthority(const std::string &authority, const std::string &url) {
	std::size_t found;
	std::size_t pos;

	found = authority.find('@');
	if (found != std::string::npos) {
		this->_userinfo = authority.substr(0, found);
		for (std::string::const_iterator it = this->_userinfo.begin(); it != this->_userinfo.end(); ++it) {
			if (!(URL::isunreserved(*it) || URL::issubdelims(*it))) {
				// Authorized %XX format
				if (*it != '%') throw std::invalid_argument("\"" + url + "\" is not a valid URL");
				++it;
				if (it == this->_userinfo.end() || !ishexdigit(*it)) throw std::invalid_argument("\"" + url + "\" is not a valid URL");
				++it;
				if (it == this->_userinfo.end() || !ishexdigit(*it)) throw std::invalid_argument("\"" + url + "\" is not a valid URL");
			}
		}
	}

	pos = found + 1;
	this->_host = authority.substr(pos);
	if (this->_host.empty()) throw std::invalid_argument("\"" + url + "\" is not a valid URL");
	found = this->_host.rfind(':');
}
