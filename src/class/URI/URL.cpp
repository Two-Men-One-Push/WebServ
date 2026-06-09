#include "./URL.hpp"
#include <string>

URL::URL(const std::string &url) {
	this->parse(url);
}

URL::URL(const URL &other)
	: _href(other._href),
	  _protocol(other._protocol),
	  _userinfo(other._userinfo),
	  _origin(other._origin),
	  _hostname(other._hostname),
	  _host(other._host),
	  _port(other._port),
	  _path(other._path),
	  _pathSegments(other._pathSegments),
	  _search(other._search),
	  _searchParams(other._searchParams),
	  _hash(other._hash) {}

URL URL::operator=(const URL &other) {
	if (this == &other) return *this;
	this->_href = other._href;
	this->_protocol = other._protocol;
	this->_userinfo = other._userinfo;
	this->_origin = other._origin;
	this->_hostname = other._hostname;
	this->_host = other._host;
	this->_port = other._port;
	this->_path = other._path;
	this->_pathSegments = other._pathSegments;
	this->_search = other._search;
	this->_searchParams = other._searchParams;
	this->_hash = other._hash;
	return *this;
}

const std::string &URL::href() const {
	return this->_href;
}

const std::string &URL::protocol() const {
	return this->_protocol;
}

const std::string &URL::userinfo() const {
	return this->_userinfo;
}

const std::string &URL::origin() const {
	return this->_origin;
}

const std::string &URL::hostname() const {
	return this->_hostname;
}

const std::string &URL::host() const {
	return this->_host;
}

const std::string &URL::port() const {
	return this->_port;
}

const std::string &URL::path() const {
	return this->_path;
}

const std::vector<std::string> &URL::pathSegments() const {
	return this->_pathSegments;
}

const std::string &URL::search() const {
	return this->_search;
}

const std::map<std::string, std::string> &URL::searchParams() const {
	return this->_searchParams;
}

const std::string &URL::hash() const {
	return this->_hash;
}
