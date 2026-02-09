#ifndef MAINBLOCK_CPP
#define MAINBLOCK_CPP

#include "config/directives/final/ListenDirective/ListenDirective.hpp"

class MainBlock {
  private:
	ListenDirective _listen;

  public:
	MainBlock(void);
	~MainBlock(void);

	static void parse(MainBlock &, const std::string &);
};

typedef MainBlock Config;

#endif
