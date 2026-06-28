#ifndef USHORTDIRECTIVE_HPP
#define USHORTDIRECTIVE_HPP

#include "config/directives/BaseDirective/BaseDirective.hpp"
#include <string>
#include <sys/types.h>

class UShortDirective : public BaseDirective<ushort> {
  public:
	UShortDirective(ushort);
	virtual ~UShortDirective();
	void parse(const std::string &);
};

#endif
