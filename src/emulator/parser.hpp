#pragma once

#include <iostream>
#include <stdlib.h>
#include <regex>

#include "command.hpp"


class Parser
{
public:
	
	Parser() = delete;

	Parser(const Parser& parser) = delete;
	Parser(Parser&& parser) = default;

	Parser& operator=(const Parser& parser) = delete;
//	Parser& operator=(Parser&& parser) = default;

	Parser(const char* filename);

	idk parse_programm();

private:

	std::ifstream file_;
	const char* pos_;
	const char* end_;

	что-то parse_line();

	что-то parse_command();
	что-то parse_command_name();

	Reg_t parse_register();

	std::string parse_label_name();
	Value_t parse_value();

	bool parse_space_seq();
	bool parse_newline_seq();




} // class Parser