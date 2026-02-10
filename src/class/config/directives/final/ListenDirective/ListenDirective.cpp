#include "./ListenDirective.hpp"
#include "defaultDirectives.hpp"
#include <sys/types.h>

ListenDirective::ListenDirective() : UShortDirective(DFLT_LISTEN) {}

ListenDirective::~ListenDirective() {}
