#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>

//#include "emulator.hpp"
class Emulator;

typedef int Value_t;
typedef uint8_t Reg_t;
typedef uint8_t Cmd_t;
constexpr int MAX_LINE_LEN = 100;

extern std::vector<std::string> reg_table, cmd_table;


Reg_t reg_id_from_name(std::string name);


enum CMD_ID: Cmd_t
{
	BEGIN = 0,
	END = 1,
	PUSH = 2,
	POP = 3,
	PUSHR = 4,
	POPR = 5,
	ADD = 6,
	SUB = 7,
	MUL = 8,
	DIV = 9,
	OUT = 10,
	IN = 11,
	JMP = 12,
	JEQ = 13,
	JNE = 14,
	JA = 15,
	JAE = 16,
	JB = 17,
	JBE = 18,
	CALL = 19,
	RET = 20
};

Cmd_t cmd_id_from_name(std::string name);

class Command
{
public:
	virtual void execute(Emulator* eml);

}; // class Command


class Cmd_BEGIN: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_END: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_PUSH: public Command
{
public:
	void execute(Emulator* eml);

	Value_t value;
};

class Cmd_POP: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_PUSHR: public Command
{
public:
	void execute(Emulator* eml);

	Reg_t reg;
};

class Cmd_POPR: public Command
{
public:
	void execute(Emulator* eml);

	Reg_t reg;
};

class Cmd_ADD: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_SUB: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_MUL: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_DIV: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_OUT: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_IN: public Command
{
public:
	void execute(Emulator* eml);
};

class Cmd_JUMP: public Command
{
public:

	int to;

	virtual bool check(Value_t val1, Value_t val2);

	virtual void execute(Emulator* eml);
};

class Cmd_JMP: public Cmd_JUMP
{
	void execute(Emulator* eml);
};

class Cmd_JEQ: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JNE: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JA: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JAE: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JB: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JBE: public Cmd_JUMP
{
	bool check(Value_t val1, Value_t val2);
};

class Cmd_CALL: public Cmd_JUMP
{
public:
	void execute(Emulator* eml);
};

class Cmd_RET: public Command
{
public:
	void execute(Emulator* eml);
};
