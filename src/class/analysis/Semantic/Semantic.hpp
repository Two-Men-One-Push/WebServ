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
	private:
		enum ArgShape  { ARGS_FORBIDDEN, ARGS_EXACT_ONE, ARGS_EXACT_TWO, ARGS_AT_LEAST_ONE, ARGS_AT_LEAST_TWO };
		enum BodyShape { BODY_FORBIDDEN, BODY_REQUIRED };

		static bool			checkShape(const Directive &d, ArgShape args, BodyShape body, DiagnosticContext &diag);
		static void			parseClientMaxBodySize(std::list<Directive>::const_iterator it, size_t &size, DiagnosticContext &diag);
		static void			parseErrorPages(std::list<Directive>::const_iterator it, std::map<int, std::pair<int, std::string> > &error_pages, DiagnosticContext &diag);
		static void			parseCGI(std::list<Directive>::const_iterator it, std::map<std::string, std::string> &cgi, DiagnosticContext &diag);
		static void			parseListen(std::list<Directive>::const_iterator it, std::vector<int> &listen, Http &http, DiagnosticContext &diag);
		static void			parseServerNames(std::list<Directive>::const_iterator it, std::vector<std::string> &server_names, Http &http, DiagnosticContext &diag);
		static void			parseRoot(std::list<Directive>::const_iterator it, std::string &root, DiagnosticContext &diag);
		static void			parseIndex(std::list<Directive>::const_iterator it, std::vector<std::string> &index_files, DiagnosticContext &diag);
		static void			parseAllowMethods(std::list<Directive>::const_iterator it, std::vector<std::string> &allowed_methods, DiagnosticContext &diag);
		static void			parseAutoindex(std::list<Directive>::const_iterator it, bool &autoindex, DiagnosticContext &diag);
		static void			parseRedirection(std::list<Directive>::const_iterator it, std::pair<int, std::string> &redirection, DiagnosticContext &diag);
		static void			parseUploadPath(std::list<Directive>::const_iterator it, std::string &upload_path, DiagnosticContext &diag);
		static void			parseTypes(std::list<Directive>::const_iterator it, MimeTypes &types, DiagnosticContext &diag);
		static void			parseHttp(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag);
		static void			parseServer(std::list<Directive>::const_iterator it, Http &http, DiagnosticContext &diag);
		template <typename Type>
		static void			parseLocation(std::list<Directive>::const_iterator it, std::vector<Location> &locations, Type &parent, DiagnosticContext &diag);

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
