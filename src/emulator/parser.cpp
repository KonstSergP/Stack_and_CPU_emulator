#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>
#include <iterator>

#include "parser.hpp"
#include "command.hpp"
#include "../../include/utils.hpp"



Parser::Parser(const char* filename):
	file_ (std::ifstream(filename, std::ios::in))
{	
	ASSERT(file_.good(), "Unable to open file \'%s\'\n", filename);

	read_line_from_file();
}

void Parser::read_line_from_file()
{
	ASSERT(file_.good(), "File is invalid\n");
	ASSERT(!file_.eof(), "EOF, cannot read line\n");

	file_.getline(line_, MAX_LINE_LEN);
	pos_ = line_;
	end_ = line_ + strlen(line_);

}

bool Parser::parse_pattern(std::regex regexp)
{

	std::cmatch match_result;

	bool success = std::regex_search(pos_, end_,
									match_result, regexp,
									std::regex_constants::match_continuous);

	if (success)
	{
		pos_ = match_result[0].second;

	}

	return success;

}

bool Parser::parse_pattern(std::regex regexp, std::string& ret)
{

	std::cmatch match_result;

	bool success = std::regex_search(pos_, end_,
									match_result, regexp,
									std::regex_constants::match_continuous);

	if (success)
	{
		pos_ = match_result[0].second;

		ret = std::string(match_result[0].first, match_result[0].second);

	}

	return success;

}


bool Parser::parse_space_seq()
{
	std::regex pat("[ \t]+");

	return parse_pattern(pat);

}

bool Parser::parse_newline_seq()
{
	bool status = (pos_ == end_);

	while ((pos_ == end_) && !file_.eof())
	{
		read_line_from_file();
	}

	return status;

}

bool Parser::parse_value(Value_t& value)
{
	parse_space_seq();

	std::regex pat("(\\+|-)?[1-9][0-9]*");

	std::string val;

	bool success = parse_pattern(pat, val);

	if (success)
	{
		value = std::stoi(val);
	}

	return success;
}

bool Parser::parse_label_name(std::string& name)
{
	parse_space_seq();

	std::regex pat("[a-zA-Z]+");

	return parse_pattern(pat, name);

}

Reg_t Parser::parse_register()
{
	parse_space_seq();

	std::regex pat("AX|BX|CX|DX|EX|FX|PC");
	std::string reg;
	parse_pattern(pat, reg);

	return reg_id_from_name(reg);
}

std::string Parser::parse_command_name()
{
	std::regex pat("PUSHR|POPR|BEGIN|END|PUSH|POP|ADD|SUB|MUL|DIV|OUT|IN|JMP|JEQ|JNE|JAE|JA|JBE|JB|CALL|RET|[a-zA-Z]+:");
	std::string com;
	parse_pattern(pat, com);

	return com;
}

void Parser::parse_command_line(Command*& ret, int& status, int number)
{
	parse_space_seq();

	std::string name = parse_command_name();
	Cmd_t cmd_id = cmd_id_from_name(name);
	Value_t val;
	Reg_t rg;

	switch (cmd_id)
	{
	case CMD_ID::BEGIN:
		ret =  new Cmd_BEGIN();
		break;
	case CMD_ID::END:
		ret = new Cmd_END();
		break;
	case CMD_ID::PUSH:
		ret = new Cmd_PUSH();

		parse_value(val);
		dynamic_cast<Cmd_PUSH*>(ret)->value = val;
		
		break;
	case CMD_ID::POP:
		ret = new Cmd_POP();
		break;
	case CMD_ID::PUSHR:
		ret = new Cmd_PUSHR();

		rg = parse_register();
		dynamic_cast<Cmd_PUSHR*>(ret)->reg = rg;

		break;
	case CMD_ID::POPR:
		ret = new Cmd_POPR();

		rg = parse_register();
		dynamic_cast<Cmd_POPR*>(ret)->reg = rg;

		break;
	case CMD_ID::ADD:
		ret = new Cmd_ADD();
		break;
	case CMD_ID::SUB:
		ret = new Cmd_SUB();
		break;
	case CMD_ID::MUL:
		ret = new Cmd_MUL();
		break;
	case CMD_ID::DIV:
		ret = new Cmd_DIV();
		break;
	case CMD_ID::OUT:
		ret = new Cmd_OUT();
		break;
	case CMD_ID::IN:
		ret = new Cmd_IN();
		break;
	case CMD_ID::JMP:
		ret = new Cmd_JMP();
		status = 1;
		break;
	case CMD_ID::JEQ:
		ret = new Cmd_JEQ();
		status = 1;
		break;
	case CMD_ID::JNE:
		ret = new Cmd_JNE();
		status = 1;
		break;
	case CMD_ID::JA:
		ret = new Cmd_JA();
		status = 1;
		break;
	case CMD_ID::JAE:
		ret = new Cmd_JAE();
		status = 1;
		break;
	case CMD_ID::JB:
		ret = new Cmd_JB();
		status = 1;
		break;
	case CMD_ID::JBE:
		ret = new Cmd_JBE();
		status = 1;
		break;
	case CMD_ID::CALL:
		ret = new Cmd_CALL();
		status = 1;
		break;
	case CMD_ID::RET:
		ret = new Cmd_RET();
		break;
	default:
		//ret = new Cmd_LABEL();
		name.pop_back();

		//dynamic_cast<Cmd_LABEL*>(ret)->name = name;
		status = 0;
		labels.push_back(std::pair<std::string, int>(name, number));

	} // switch

	if (status == 1)
	{
		std::string lbl;
		parse_label_name(lbl);
		jumps.push_back(std::pair<std::string, int>(lbl, number));
	}

	parse_space_seq();
	parse_newline_seq();

} // parse_command


std::vector<Command*> Parser::parse_programm()
{
	std::vector<Command*> vec;
	Command* cm;
	int res = 2;
	int i = 0;
	while (!file_.eof())
	{	
		res = 2;
		parse_command_line(cm, res, i);

		if (res)
		{
			vec.push_back(cm);
			i++;
		}
	}


	for (size_t i = 0; i < jumps.size(); i++)
	{
		for (size_t j = 0; j < labels.size(); j++)
		{
			if (jumps[i].first == labels[j].first)
			{
				dynamic_cast<CMD_JUMP*>(vec[jumps[i].second])->to = labels[j].second;
			}
		}
	}

	return vec;
}
