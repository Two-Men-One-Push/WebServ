#pragma once

#include "http/messages/request/HttpRequest.hpp"
#include "model/Server/Server.hpp"
#include "Ressource/Ressource.hpp"

class	Router
{
	private:
	public:
		Router();
		~Router();

		static Ressource	resolveRessource(const HttpRequest &req, const Server &server);
};