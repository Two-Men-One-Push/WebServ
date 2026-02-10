#include "./BooleanDirective.hpp"
#include <stdexcept>
#include <string>

BooleanDirective::BooleanDirective(bool defaultValue) : BaseDirective(defaultValue) {}

void BooleanDirective::parse(const std::string &input) {
	if (input == "yes") {
		_value = true;
	} else if (input == "no") {
		_value = false;
	} else {
		throw std::runtime_error("Invalid input :(");
	}
}
