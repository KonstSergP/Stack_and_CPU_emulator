#include <iostream>
#include <stdlib.h>
#include <cstring>

#include "../src/emulator/parser.hpp"
#include "../src/emulator/command.hpp"
#include "../src/emulator/emulator.hpp"
#include "../include/myStack.hpp"

int main(int argc, char **argv)
{
	std::string filename;
	if (argc == 1)
    {
        std::cout << "Файл с программой: ";
        std::cin >> filename;
    }
    else if (argc == 2)
    {
        filename = argv[1];
    }
    else
    {
    	std::cout << "Wrong number of arguments\n";
    }

    Parser parser((std::string("build/") + filename + std::string(".bc")).c_str());

    Emulator eml(parser.load_binary((std::string("build/") + filename + std::string(".bc")).c_str()));

    eml.run();

	return 0;
}
