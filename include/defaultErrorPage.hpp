#include "http/HttpStatus.hpp"
#include <ostream>

std::ostream	&operator<<(std::ostream &os, HttpStatus::Code code);
void generateErrorPage(std::ostream &os, HttpStatus::Code code);
