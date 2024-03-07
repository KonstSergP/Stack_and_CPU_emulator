#include <iostream>
#include <stdlib.h>
#include "command.hpp"

#include "emulator.hpp"

std::vector<std::string> reg_table = 
{
	"AX",
	"BX",
	"CX",
	"DX",
	"EX",
	"FX",
	"PC"
};

std::vector<std::string> cmd_table = 
{
	"BEGIN",
	"END",
	"PUSH",
	"POP",
	"PUSHR",
	"POPR",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"OUT",
	"IN",
	"JMP",
	"JEQ",
	"JNE",
	"JA",
	"JAE",
	"JB",
	"JBE",
	"CALL",
	"RET",
	"label"
};


Reg_t reg_id_from_name(std::string name)
{
	for (size_t i = 0; i < reg_table.size(); i++)
	{
		if (name == reg_table[i])
		{
			return i;
		}
	}

	return -1;
}

Cmd_t cmd_id_from_name(std::string name)
{
	for (size_t i = 0; i < cmd_table.size(); i++)
	{
		if (name == cmd_table[i])
		{
			return i;
		}
	}

	return -1;
}



void Command::execute(Emulator* eml)
{
	std::cout << eml->registers[6] << "\n";
}

void Cmd_BEGIN::execute(Emulator* eml)
{
	//printf("BEGIN\n");
	eml->mode = true;
}

void Cmd_END::execute(Emulator* eml)
{
	//printf("END\n");
	if (eml->mode)
	{
		eml->mode = false;
		eml->registers[6] = eml->programm.size();
	}
}

void Cmd_PUSH::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("PUSH %d\n", value);
	eml->stack.push(value);
}

void Cmd_POP::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("POP\n");
	eml->stack.pop();
}

void Cmd_PUSHR::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//std::cout << "PUSHR " << reg_table[reg] << "\n";
	eml->stack.push(eml->registers[reg]);
}

void Cmd_POPR::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//std::cout << "POPR " << reg_table[reg] << "\n";
	Value_t val = eml->stack.top();
	eml->stack.pop();
	eml->registers[reg] = val;
}

void Cmd_ADD::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("ADD\n");
	Value_t val1 = eml->stack.top();
	eml->stack.pop();
	Value_t val2 = eml->stack.top();
	eml->stack.pop();
	eml->stack.push(val1 + val2);
}

void Cmd_SUB::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("SUB\n");
	Value_t val1 = eml->stack.top();
	eml->stack.pop();
	Value_t val2 = eml->stack.top();
	eml->stack.pop();
	eml->stack.push(val1 - val2);
}

void Cmd_MUL::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("MUL\n");
	Value_t val1 = eml->stack.top();
	eml->stack.pop();
	Value_t val2 = eml->stack.top();
	eml->stack.pop();
	eml->stack.push(val1 * val2);
}

void Cmd_DIV::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("DIV\n");
	Value_t val1 = eml->stack.top();
	eml->stack.pop();
	Value_t val2 = eml->stack.top();
	eml->stack.pop();
	eml->stack.push(val1 / val2);
}

void Cmd_OUT::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("OUT\n");
	Value_t val = eml->stack.top();
	eml->stack.pop();
	std::cout << val << "\n";
}

void Cmd_IN::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("IN\n");
	Value_t val;
	std::cin >> val;
	eml->stack.push(val);
}

bool Cmd_JUMP::check(Value_t val1, Value_t val2)
{
	return (val1 == val2) || (val1 != val2);
}

void Cmd_JUMP::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	Value_t val1 = eml->stack.top();
	eml->stack.pop();
	Value_t val2 = eml->stack.top();
	eml->stack.push(val1);

	if (check(val1, val2))
	{
		eml->registers[6] = to - 1;
		//std::cout << "JUMP " << to << "\n";
	}
}

void Cmd_JMP::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	eml->registers[6] = to - 1;
	//std::cout << "JUMP " << to << "\n";
}

bool Cmd_JEQ::check(Value_t val1, Value_t val2)
{
	return val1 == val2;
}

bool Cmd_JNE::check(Value_t val1, Value_t val2)
{
	return val1 != val2;
}

bool Cmd_JA::check(Value_t val1, Value_t val2)
{
	return val1 > val2;
}

bool Cmd_JAE::check(Value_t val1, Value_t val2)
{
	return val1 >= val2;
}

bool Cmd_JB::check(Value_t val1, Value_t val2)
{
	return val1 < val2;
}

bool Cmd_JBE::check(Value_t val1, Value_t val2)
{
	return val1 <= val2;
}


void Cmd_CALL::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//std::cout << "CALL " << to << "\n";
	eml->stack.push(eml->registers[6]);
	eml->registers[6] = to - 1;
}

void Cmd_RET::execute(Emulator* eml)
{
	if (!eml->mode) {return;}
	//printf("RET\n");
	Value_t to = eml->stack.top();
	eml->stack.pop();
	eml->registers[6] = to;
}
