
#include "WebServer/WebServer.hpp"
#include "config/MainContext/MainContext.hpp"
#include <exception>
#include <iostream>


int main(void) {
	try {
		Config config;
		config.parse(config, "bonjour");
		WebServer webServer(config);
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}
