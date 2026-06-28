#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include <string>

class HttpException : public std::exception {
  protected:
	std::string _message;
	HttpStatus::Code _status;

  public:
	HttpException(HttpStatus::Code status) throw();

	virtual ~HttpException() throw();
	virtual const char *what() const throw();
	HttpStatus::Code status() const;
	const std::string &message() const;
};

#endif
