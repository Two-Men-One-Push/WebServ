#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include "http/HttpStatus.hpp"
#include <exception>
#include <string>

template <HttpStatus::Code status> class HttpError : std::exception {
  private:
	std::string _message;

  public:
	HttpError();
	virtual ~HttpError() throw();
	virtual const char *what() const throw();
};

namespace http_errors {

};

#endif
