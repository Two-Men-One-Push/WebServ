#ifndef WEBSERVERRORS_HPP
#define WEBSERVERRORS_HPP

#include <cerrno>
#include <cstdio>
#include <exception>
#include <string>

namespace webserv_errors {
class LoggableError : public std::exception {
  public:
	virtual ~LoggableError() throw();

	// virtual void print(std::ostream &) const = 0;
};

// Logger &operator<<(std::ostream &logger, const LoggableError &e) {
// 	e.print(logger);
// 	return logger;
// }

class SysError : public LoggableError {
  private:
	int _err;
	const std::string _context;
	const std::string _cause;
	const std::string _msg;

  public:
	SysError(int err);
	SysError(const std::string &context, int err);
	SysError(const std::string &context, int err, const std::string &cause);
	~SysError() throw();

	virtual const char *what() const throw();

	int getErr() const;
};

}; // namespace webserv_errors

#endif
