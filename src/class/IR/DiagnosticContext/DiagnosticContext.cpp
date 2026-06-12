#include "DiagnosticContext.hpp"
#include <iostream>
#include <sstream>

DiagnosticContext::DiagnosticContext(): _has_error(false)
{
}

void	DiagnosticContext::report(const std::string &message, const ErrorInfo &location)
{
	std::cerr
		<< location.getFilename() << ":"
		<< location.getLineNumber() << ":"
		<< location.getColumnNumber() << ": error: "
		<< message << std::endl;
	_has_error = true;
}

bool	DiagnosticContext::hasError() const
{
	return _has_error;
}
