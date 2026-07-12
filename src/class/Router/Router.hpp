#pragma once

#include "model/Server/Server.hpp"
#include "Ressource/Ressource.hpp"
#include "URL/URL.hpp"

class	Router
{
	private:
	public:
		Router();
		~Router();

		static Ressource	resolveRessource(const URL &url, const Server &server);
};