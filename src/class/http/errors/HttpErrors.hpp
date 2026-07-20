#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include <string>

class HttpError : public std::exception {
  protected:
	std::string _message;
	HttpStatus::Code _status;

  public:
	HttpError(HttpStatus::Code status) throw();
	HttpError(HttpStatus::Code status, const std::string &detail) throw();

	virtual ~HttpError() throw();
	virtual const char *what() const throw();
	HttpStatus::Code status() const;
	const std::string &message() const;
};

#endif
