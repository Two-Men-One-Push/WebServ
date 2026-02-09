#include "config/directives/UShortDirective/UShortDirective.hpp"
#include "config/directives/BaseDirective/BaseDirective.hpp"
#include <cstdlib>
#include <sys/types.h>

UShortDirective::UShortDirective(ushort defaultValue) : BaseDirective(defaultValue) {}

UShortDirective::~UShortDirective() {}

void UShortDirective::parse(const std::string &input) {
	_value = std::atoi(input.c_str());
}
