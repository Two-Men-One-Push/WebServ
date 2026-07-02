#pragma once

#include "IR/ErrorInfo/ErrorInfo.hpp"
#include <string>

class DiagnosticContext
{
	bool	_has_error;
public:
	DiagnosticContext();

	void	report(const std::string &message, const ErrorInfo &location);
	bool	hasError() const;
};
