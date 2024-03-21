#pragma once
#include <fstream>
#include <iterator>
#include <iostream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <map>
#include <cstring>

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

	std::vector<Command*> parse_program();
	std::vector<Command*> load_binary(const char* filename);

	void save(const char* filename);

private:

	std::ifstream file_;
	const char* pos_;
	const char* end_;
	char line_[MAX_LINE_LEN];
	size_t line_counter_;
	std::map<std::string, int> labels_;
	std::map<int, std::string> jumps_;
	std::vector<Command*> program;

	bool parse_pattern(std::regex regexp);
	bool parse_pattern(std::regex regexp, std::string& ret);

	void read_line_from_file();

	void parse_command_line(Command*& ret, int& status, int number);
	bool parse_command_name(std::string& name);

	bool parse_register(Reg_t& rg);
	bool parse_label_name(std::string& name);
	bool parse_value(Value_t& val);

	bool parse_space_seq();
	bool parse_newline_seq();
	bool parse_comment();

	void ASSERT_PARS(bool contract, const char* msg);


}; // class Parser
