#include "defaultErrorPage.hpp"

void printDefaultErrorPage(std::ostream &os, HttpStatus::Code code, const std::string &message) {
	os << "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
<meta charset=\"UTF-8\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
<title>An Error Occured</title>\
</head>\
<body>" << int(code) << ": " << message << "</body>\
</html>";
}

void printDefaultErrorPage(std::ostream &os, HttpStatus::Code code) {
	os << "<!DOCTYPE html>\
<html lang=\"en\">\
<head>\
<meta charset=\"UTF-8\">\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
<title>An Error Occured</title>\
</head>\
<body>" << int(code) << ": " << HttpStatus::reasonPhrase(code) << "</body>\
</html>";
}
