#pragma once

#include "http/messages/request/HttpRequest.hpp"
#include "model/Server/Server.hpp"
#include "Ressource/Ressource.hpp"
#include "http/HttpStatus.hpp"

class	Router
{
	private:
	public:
		Router();
		~Router();

		static Ressource	resolveRessource(const HttpRequest &req, const Server &server);
		static Ressource	resolveErrorRessource(const HttpRequest &req, HttpStatus::Code errorCode, const Server &server);
};