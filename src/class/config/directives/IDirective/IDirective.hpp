#ifndef IDIRECTIVE_HPP
#define IDIRECTIVE_HPP

#include <string>

class IDirective {
  public:
	virtual ~IDirective() {}

	virtual void parse(const std::string &) = 0;
};

#endif
