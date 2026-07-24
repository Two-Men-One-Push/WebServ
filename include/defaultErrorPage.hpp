#include "http/HttpStatus.hpp"
#include <ostream>

void printDefaultErrorPage(std::ostream &os, HttpStatus::Code code, const std::string &message);
void printDefaultErrorPage(std::ostream &os, HttpStatus::Code code);
