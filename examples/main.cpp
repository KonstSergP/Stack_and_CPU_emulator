#include <iostream>
#include <stdlib.h>

#include "../src/emulator/parser.hpp"
#include "../src/emulator/command.hpp"
#include "../src/emulator/emulator.hpp"
#include "../include/myStack.hpp"

int main()
{

    Parser parser("factorial_rec.test");

    Emulator eml(parser.parse_programm());
    
    eml.run();

	return 0;
}
