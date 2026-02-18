#include <bits/types/error_t.h>
#include <cerrno>
#include <cstdio>
#include <exception>
#include <ostream>

namespace webserv_errors {
class LoggableError : std::exception {
  public:
	virtual std::ostream &operator<<(std::ostream &) = 0;
};

class SystemError : LoggableError {
  private:
	const std::string _context;
	const std::string _cause;
	int _err;

  public:
	SystemError(int err);
	SystemError(const std::string &context, int err);
	SystemError(const std::string &context, int err, const std::string &cause);
	~SystemError() throw();

	std::ostream &operator<<(std::ostream &);
	const char *what() const throw();
};

}; // namespace webserv_errors
