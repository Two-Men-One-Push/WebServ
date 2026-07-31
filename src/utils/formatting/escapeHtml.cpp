#include <ostream>
#include <string>

void printEscapedHtml(std::ostream &os, const std::string &htmlText) {
	for (std::string::const_iterator it = htmlText.begin(); it != htmlText.end(); ++it) {
		switch (*it) {
        case '&': os << "&amp;"; break;
        case '<': os << "&lt;"; break;
        case '>': os << "&gt;"; break;
        case '"': os << "&quot;"; break;
        case '\'': os << "&#39;"; break;
        default: os << *it;
        }
	}
}
