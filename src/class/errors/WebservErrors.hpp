#ifndef WEBSERVERRORS_HPP
#define WEBSERVERRORS_HPP

#include <cerrno>
#include <cstdio>
#include <exception>
#include <string>

namespace WebservErrors {
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
	const std::string _message;

  public:
	SysError(int err) throw();
	SysError(const std::string &context, int err) throw();
	SysError(const std::string &context, int err, const std::string &cause) throw();
	SysError(const SysError &other) throw();
	~SysError() throw();

	virtual const char *what() const throw();

	int err() const;
};

class GaiError : public LoggableError {
  private:
	int _err;
	const std::string _context;
	const std::string _cause;
	const std::string _message;

  public:
	GaiError(int err) throw();
	GaiError(const std::string &context, int err) throw();
	GaiError(const std::string &context, int err, const std::string &cause) throw();
	GaiError(const GaiError &other) throw();
	~GaiError() throw();

	virtual const char *what() const throw();

	int getErr() const;
};

class Runtime : public LoggableError {
  private:
	const std::string _message;

  public:
	Runtime(const std::string &message) throw();
	Runtime(const Runtime &other) throw();
	~Runtime() throw();

	virtual const char *what() const throw();
};

}; // namespace WebservErrors

#endif
