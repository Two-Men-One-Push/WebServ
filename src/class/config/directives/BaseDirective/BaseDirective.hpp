#ifndef ADIRECTIVE_HPP
#define ADIRECTIVE_HPP

#include "config/directives/IDirective/IDirective.hpp"

template <typename T>
class BaseDirective : public IDirective {
  protected:
	T _value;

  public:
	BaseDirective(const T &defaultValue) : _value(defaultValue) {}

	virtual ~BaseDirective() {}

	const T &get(void) const {
		return _value;
	}

	void set(const T &newValue) {
		_value = newValue;
	}
};

#endif
