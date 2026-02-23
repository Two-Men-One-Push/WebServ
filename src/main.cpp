
#include "WebServer/WebServer.hpp"
#include "config/MainContext/MainContext.hpp"


int main(void) {
	Config config;
	config.parse(config, "bonjour");
	WebServer webServer(config);
}
