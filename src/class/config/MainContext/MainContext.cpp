#include "./MainContext.hpp"
#include <map>

MainContext::MainContext(void) {}

MainContext::~MainContext(void) {}

void MainContext::parse(MainContext &block, const std::string &) {
	std::map<std::string, IDirective *> directiveMap;

	directiveMap["test1"] = &block._listen;
}
