#include <iostream>
#include <stdlib.h>
#include <iterator>

#include "../../include/utils.hpp"
#include "../../include/myStack.hpp"
#include "parser.hpp"
#include "command.hpp"
#include "emulator.hpp"

using namespace handMadeStack;

Emulator::Emulator(const std::vector<Command*>& vec)
{
	programm = vec;
	memset(registers, 0, sizeof(int)*7);
	mode = false;
}

void Emulator::run()
{	
	std::cout << programm.size() << "\n";
	printf("Execution start\n");
	while (registers[6] < static_cast<int>(programm.size()))
	{
		std::cout << registers[6] << " ";
		programm[registers[6]]->execute(this);
		registers[6]++;
	}

	printf("Execution end\n");


}