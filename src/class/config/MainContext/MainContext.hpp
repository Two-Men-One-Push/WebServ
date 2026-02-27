#ifndef MAINCONTEXT_CPP
#define MAINCONTEXT_CPP

#include "config/directives/final/ListenDirective/ListenDirective.hpp"

class MainContext {
  private:
	ListenDirective _listen;

  public:
	MainContext(void);
	~MainContext(void);

	static void parse(MainContext &, const std::string &);
};

typedef MainContext Config;

#endif
