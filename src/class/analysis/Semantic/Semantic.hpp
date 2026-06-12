#pragma once

#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include "Parser.hpp"
#include "MimeType.hpp"
#include "Http.hpp"
#include <vector>

class	Semantic
{
	private:
		
		static Http		analyseHttp(const std::list<Directive> &directives);
		static Server	analyseServer(const std::list<Directive> &directives);
		static Location	analyseLocation(const std::list<Directive> &directives);
		static MimeType	analyseMimeType(const std::list<Directive> &directives);
		static void		analyseDirective(const Directive &directive, Config &config);
	public:
		Semantic();
		~Semantic();

		static Config	analyseAST(const AST &ast);

		class	SemanticError: public std::exception
		{
			public:
				virtual ~SemanticError() throw();
				virtual const char	*what() const throw();
		};
};