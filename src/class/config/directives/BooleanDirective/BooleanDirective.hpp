#ifndef BOOLEANDIRECTIVE_HPP
#define BOOLEANDIRECTIVE_HPP

#include "config/directives/BaseDirective/BaseDirective.hpp"

class BooleanDirective : public BaseDirective<bool> {
  public:
	BooleanDirective(bool defaultValue);
	void parse(const std::string &input);
};

#endif
