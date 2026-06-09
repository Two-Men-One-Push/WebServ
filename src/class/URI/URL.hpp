#ifndef URLHPP
#define URLHPP

#include <map>
#include <string>
#include <vector>

class URL {
  private:
	std::string _href;

	std::string _protocol;

	std::string _userinfo;

	std::string _origin;
	std::string _hostname;
	std::string _host;
	std::string _port;

	std::string _path;
	std::vector<std::string> _pathSegments;

	std::string _search;
	std::map<std::string, std::string> _searchParams;

	std::string _hash;

	void parse(const std::string &url);
	void parseAuthority(const std::string &authority, const std::string &url);

  public:
	URL(const std::string &url);
	URL(const URL &other);
	URL operator=(const URL &other);

	const std::string &href() const;

	const std::string &protocol() const;

	const std::string &userinfo() const;

	const std::string &origin() const;
	const std::string &hostname() const;
	const std::string &host() const;
	const std::string &port() const;

	const std::string &path() const;
	const std::vector<std::string> &pathSegments() const;

	const std::string &search() const;
	const std::map<std::string, std::string> &searchParams() const;

	const std::string &hash() const;

	static bool isunreserved(char c);
	static bool issubdelims(char c);
};

#endif
