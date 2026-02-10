#ifndef MAINBLOCK_HPP
#define MAINBLOCK_HPP

#include "./MainBlock.hpp"
#include <map>

MainBlock::MainBlock(void) {}

MainBlock::~MainBlock(void) {}

void MainBlock::parse(MainBlock &block, const std::string &) {
	std::map<std::string, IDirective *> directiveMap;

	directiveMap["test1"] = &block._listen;
}

#endif
