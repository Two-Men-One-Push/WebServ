#pragma once

#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Directive.hpp"
#include "Parser.hpp"
#include <vector>

class	Semantic
{
	private:
		
		static Server	analyseServer(const std::list<Directive> &directives);
		static Location	analyseLocation(const std::list<Directive> &directives);
	public:
		Semantic();
		~Semantic();

		static Config	analyseAST(const AST &ast);

		class	SemanticUnknownDirective: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~SemanticUnknownDirective() throw();
				SemanticUnknownDirective(const std::string &directive, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
		class	SemanticInvalidArguments: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~SemanticInvalidArguments() throw();
				SemanticInvalidArguments(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
		class	SemanticIllegalBody: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~SemanticIllegalBody() throw();
				SemanticIllegalBody(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
		class	SemanticBodyNotSpecified: public std::exception
		{
			private:
				std::string	_message;
			public:
				virtual ~SemanticBodyNotSpecified() throw();
				SemanticBodyNotSpecified(const std::string &description, const ErrorInfo &error_info);
				virtual const char	*what() const throw();
		};
};