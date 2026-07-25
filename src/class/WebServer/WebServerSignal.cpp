#include "./WebServer.hpp"
#include <csignal>

sig_atomic_t WebServer::running = true;
sig_atomic_t WebServer::signal = 0;

void WebServer::sigintHandler(int) {
	WebServer::running = false;
	WebServer::signal = SIGINT;
}
