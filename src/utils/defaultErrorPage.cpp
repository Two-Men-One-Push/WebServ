#include "defaultErrorPage.hpp"

std::ostream	&operator<<(std::ostream &os, HttpStatus::Code code)
{
	os << "<!DOCTYPE html>" << std::endl;
	os << "<html lang=\"en\">" << std::endl;
	os << "<head>" << std::endl;
	os << "\t<meta charset=\"UTF-8\">" << std::endl;
	os << "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">" << std::endl;
	os << "\t<title>An Error Occured</title>" << std::endl;
	os << "</head>" << std::endl;
	os << "<body>" << std::endl;
	os << "\t" << int(code) << " " << HttpStatus::reasonPhrase(code) << std::endl;
	os << "</body>" << std::endl;
	os << "</html>" << std::endl;
	return os;
}