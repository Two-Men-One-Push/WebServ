#ifndef LISTENDIRECTIVE
#define LISTENDIRECTIVE

#include "config/directives/UShortDirective/UShortDirective.hpp"
#include <sys/types.h>

class ListenDirective : public UShortDirective {
  public:
	ListenDirective();
	~ListenDirective();
};

#endif
