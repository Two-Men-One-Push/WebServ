#pragma once

#include "AST.hpp"
#include "Config.hpp"
#include "Http.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "MimeTypes.hpp"
#include "Directive.hpp"
#include "DiagnosticContext.hpp"
#include <list>

class Semantic
{
	enum ArgShape  { ARGS_FORBIDDEN, ARGS_EXACT_ONE, ARGS_EXACT_TWO, ARGS_AT_LEAST_ONE, ARGS_AT_LEAST_TWO };
	enum BodyShape { BODY_FORBIDDEN, BODY_REQUIRED };

	static bool		checkShape(const Directive &d, ArgShape args, BodyShape body, DiagnosticContext &diag);

	static Http			analyseHttp(const std::list<Directive> &directives, DiagnosticContext &diag);
	static Server		analyseServer(const std::list<Directive> &directives, Http &http, DiagnosticContext &diag);
	template <typename Type>
	static Location		analyseLocation(const std::list<Directive> &directives, Type &parent, const std::string &path, DiagnosticContext &diag);
	static void			analyseTypes(const std::list<Directive> &directives, MimeTypes &types, DiagnosticContext &diag);
public:
	Semantic();
	~Semantic();

	static Config	analyseAST(const AST &ast, DiagnosticContext &diag);
};
