#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <cstdint>
#include <fstream>

class Emulator;

typedef int Value_t;
typedef uint8_t Reg_t;
typedef uint8_t Cmd_t;
constexpr int MAX_LINE_LEN = 100;

extern std::vector<std::string> reg_table, cmd_table;

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

Reg_t reg_id_from_name(std::string name);
Cmd_t cmd_id_from_name(std::string name);

class Command
{
public:
	Cmd_t id = 255;
	virtual void execute(Emulator* eml);
	virtual void serialize(std::ofstream& out);

}; // class Command


class Cmd_BEGIN: public Command
{
public:
	Cmd_BEGIN() {id = CMD_ID::BEGIN;}
	void execute(Emulator* eml);
};

class Cmd_END: public Command
{
public:
	Cmd_END() {id = CMD_ID::END;}
	void execute(Emulator* eml);
};

class Cmd_PUSH: public Command
{
public:
	Cmd_PUSH() {id = CMD_ID::PUSH;}
	void execute(Emulator* eml);
	void serialize(std::ofstream& out);

	Value_t value;
};

class Cmd_POP: public Command
{
public:
	Cmd_POP() {id = CMD_ID::POP;}
	void execute(Emulator* eml);
};

class Cmd_PUSHR: public Command
{
public:
	Cmd_PUSHR() {id = CMD_ID::PUSHR;}
	void execute(Emulator* eml);
	void serialize(std::ofstream& out);
	Reg_t reg;
};

class Cmd_POPR: public Command
{
public:
	Cmd_POPR() {id = CMD_ID::POPR;}
	void execute(Emulator* eml);
	void serialize(std::ofstream& out);
	Reg_t reg;
};

class Cmd_ADD: public Command
{
public:
	Cmd_ADD() {id = CMD_ID::ADD;}
	void execute(Emulator* eml);
};

class Cmd_SUB: public Command
{
public:
	Cmd_SUB() {id = CMD_ID::SUB;}
	void execute(Emulator* eml);
};

class Cmd_MUL: public Command
{
public:
	Cmd_MUL() {id = CMD_ID::MUL;}
	void execute(Emulator* eml);
};

class Cmd_DIV: public Command
{
public:
	Cmd_DIV() {id = CMD_ID::DIV;}
	void execute(Emulator* eml);
};

class Cmd_OUT: public Command
{
public:
	Cmd_OUT() {id = CMD_ID::OUT;}
	void execute(Emulator* eml);
};

class Cmd_IN: public Command
{
public:
	Cmd_IN() {id = CMD_ID::IN;}
	void execute(Emulator* eml);
};

class Cmd_JUMP: public Command
{
public:
	int to;

	virtual bool check(Value_t val1, Value_t val2);
	void serialize(std::ofstream& out);
	void execute(Emulator* eml);
};

class Cmd_JMP: public Cmd_JUMP
{
public:
	Cmd_JMP() {id = CMD_ID::JMP;}
	void execute(Emulator* eml) override;
};

class Cmd_JEQ: public Cmd_JUMP
{
public:
	Cmd_JEQ() {id = CMD_ID::JEQ;}
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JNE: public Cmd_JUMP
{
public:
	Cmd_JNE() {id = CMD_ID::JNE;}
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JA: public Cmd_JUMP
{
public:
	Cmd_JA() {id = CMD_ID::JA;}
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JAE: public Cmd_JUMP
{
public:
	Cmd_JAE() {id = CMD_ID::JAE;}
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JB: public Cmd_JUMP
{
public:
	Cmd_JB() {id = CMD_ID::JB;}
	bool check(Value_t val1, Value_t val2);
};

class Cmd_JBE: public Cmd_JUMP
{
public:
	Cmd_JBE() {id = CMD_ID::JBE;}
	bool check(Value_t val1, Value_t val2);
};


// CALL inherits from Cmd_JUMP because parser will not know
// is it call or another jump
class Cmd_CALL: public Cmd_JUMP
{
public:
	Cmd_CALL() {id = CMD_ID::CALL;}
	void execute(Emulator* eml) override;
};

class Cmd_RET: public Command
{
public:
	Cmd_RET() {id = CMD_ID::RET;}
	void execute(Emulator* eml);
};
