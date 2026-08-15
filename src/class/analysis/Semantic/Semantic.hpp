#pragma once

#include "IR/AST/AST.hpp"
#include "model/Config/Config.hpp"
#include "model/Http/Http.hpp"
#include "model/Server/Server.hpp"
#include "model/Location/Location.hpp"
#include "model/MimeTypes/MimeTypes.hpp"
#include "IR/Directive/Directive.hpp"
#include "model/DiagnosticContext/DiagnosticContext.hpp"
#include <list>
#include <set>
#include <vector>

class Semantic
{
	private:
		Semantic();

		enum ArgShape  { ARGS_FORBIDDEN, ARGS_EXACT_ONE, ARGS_EXACT_TWO, ARGS_AT_LEAST_ONE, ARGS_AT_LEAST_TWO };
		enum BodyShape { BODY_FORBIDDEN, BODY_REQUIRED };

		static bool			checkShape(const Directive &d, ArgShape args, BodyShape body, DiagnosticContext &diag);
		static void			parseClientMaxBodySize(std::list<Directive>::const_iterator it, size_t &size, DiagnosticContext &diag);
		static void			parseErrorPages(std::list<Directive>::const_iterator it, std::map<HttpStatus::Code, std::pair<HttpStatus::Code, std::string> > &error_pages, DiagnosticContext &diag);
		static void			parseCGI(std::list<Directive>::const_iterator it, std::map<std::string, std::string> &cgi, DiagnosticContext &diag);
		static void			parseListen(std::list<Directive>::const_iterator it, std::vector<std::pair<std::string, int> > &listen, DiagnosticContext &diag);
		static void			parseRoot(std::list<Directive>::const_iterator it, std::string &root, DiagnosticContext &diag);
		static void			parseAlias(std::list<Directive>::const_iterator it, std::string &alias, DiagnosticContext &diag);
		static void			parseIndex(std::list<Directive>::const_iterator it, std::vector<std::string> &index_files, DiagnosticContext &diag);
		static void			parseAllowMethods(std::list<Directive>::const_iterator it, std::vector<HttpMethod> &allowed_methods, DiagnosticContext &diag);
		static void			parseAutoindex(std::list<Directive>::const_iterator it, bool &autoindex, DiagnosticContext &diag);
		static void			parseRedirection(std::list<Directive>::const_iterator it, std::string &redirection, DiagnosticContext &diag);
		static void			parseEditable(std::list<Directive>::const_iterator it, bool &editable, DiagnosticContext &diag);
		static void			parseTypes(std::list<Directive>::const_iterator it, MimeTypes &types, DiagnosticContext &diag);
		static void			parseHttp(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag);
		static void			parseServer(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag);
		static void			parseLocation(std::list<Directive>::const_iterator it, std::vector<Location> &locations, Location &parent, std::set<std::string> &locationPathTable, DiagnosticContext &diag);
		static void			parseTimeout(std::list<Directive>::const_iterator it, std::time_t &timeout, DiagnosticContext &diag);
		static Http			analyseHttp(const Directive &directive, DiagnosticContext &diag);
		static Server		analyseServer(const Directive &directive, Http &http, DiagnosticContext &diag);
		static Location		analyseLocation(const Directive &directive, std::vector<Location> &locations, Location &parent, const std::string &path, std::set<std::string> &locationPathTable, DiagnosticContext &diag);
		static void			analyseTypes(const Directive &directive, MimeTypes &types, DiagnosticContext &diag);
	public:
		~Semantic();
		static Config	analyseAST(const AST &ast, DiagnosticContext &diag);
};
