#pragma once

#include "AST.hpp"
#include "Config.hpp"
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "MimeType.hpp"
#include "Directive.hpp"
#include "DiagnosticContext.hpp"
#include <list>

class Semantic
{
	enum ArgShape  { ARGS_FORBIDDEN, ARGS_REQUIRED, ARGS_EXACT_ONE };
	enum BodyShape { BODY_FORBIDDEN, BODY_REQUIRED };

	static bool		checkShape(const Directive &d, ArgShape args, BodyShape body, DiagnosticContext &diag);

	static Http		analyseHttp(const std::list<Directive> &directives, DiagnosticContext &diag);
	static Server	analyseServer(const std::list<Directive> &directives, DiagnosticContext &diag);
	static Location	analyseLocation(const std::list<Directive> &directives, DiagnosticContext &diag);
	static MimeType	analyseMimeType(const std::list<Directive> &directives, DiagnosticContext &diag);
public:
	Semantic();
	~Semantic();

	static Config	analyseAST(const AST &ast, DiagnosticContext &diag);
};
