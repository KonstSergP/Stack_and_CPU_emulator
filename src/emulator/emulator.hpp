#pragma once
#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>
#include <iterator>

#include "command.hpp"
#include "../../include/utils.hpp"
#include "../../include/myStack.hpp"

using namespace handMadeStack;

class Emulator
{
public:
	std::vector<Command*> program;
	myStack<Value_t> stack;
	int registers[7];
	bool mode;

	Emulator(const std::vector<Command*>& vec);

	void run();
};