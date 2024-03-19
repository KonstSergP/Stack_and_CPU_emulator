#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>
#include <iterator>
#include <map>

#include "parser.hpp"
#include "command.hpp"
#include "../../include/utils.hpp"


void Parser::ASSERT_PARS(bool contract, const char* msg)
{
	if (contract) {return;}

	long int diff = pos_ - line_;
	printf("Parsing error in line %lu:%ld\n", line_counter_, diff+1);
	printf("%s\n", msg);

	size_t ln = strlen(line_);
    for (size_t i = 0; i < ln; i++)
    {
        if (line_[i] == '\t') {line_[i] = ' ';}
    }

	printf("%s\n", line_);
	std::cout << std::string(diff, ' ') << "^\n";
	abort();
}


Parser::Parser(const char* filename):
	file_ (std::ifstream(filename, std::ios::in))
{	
	ASSERT(file_.good(), "Unable to open file \'%s\'\n", filename);

	line_counter_ = 0;
	read_line_from_file();
}

void Parser::read_line_from_file()
{
	ASSERT(file_.good(), "File is invalid\n");
	ASSERT(!file_.eof(), "EOF, cannot read line\n");

	file_.getline(line_, MAX_LINE_LEN);
	line_counter_++;
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

bool Parser::parse_comment()
{
	std::regex pat("[ \t]*\\/\\/[^\n]*");

	return parse_pattern(pat);
}

bool Parser::parse_newline_seq()
{
	bool status = (pos_ == end_);

	while ((pos_ == end_ || parse_comment() || (parse_space_seq() && pos_ == end_)) && !file_.eof())
	{
		read_line_from_file();
	}

	return status;

}

bool Parser::parse_value(Value_t& value)
{

	std::regex pat("0|(\\+|-)?[1-9][0-9]*");

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

	std::regex pat("[a-zA-Z0-9_]+");

	return parse_pattern(pat, name);

}

bool Parser::parse_register(Reg_t& rg)
{

	std::regex pat("AX|BX|CX|DX|EX|FX|PC");
	std::string reg;
	bool success = parse_pattern(pat, reg);

	if (success)
	{
		rg = reg_id_from_name(reg);
	}

	return success;
}

bool Parser::parse_command_name(std::string& st)
{
	std::regex pat("PUSHR|POPR|BEGIN|END|PUSH|POP|ADD|SUB|MUL|DIV|OUT|IN|JMP|JEQ|JNE|JAE|JA|JBE|JB|CALL|RET|[a-zA-Z0-9_]+:");
	return parse_pattern(pat, st);
}

void Parser::parse_command_line(Command*& ret, int& status, int number)
{
	parse_newline_seq();
	parse_space_seq();
	std::string name;
	ASSERT_PARS(parse_command_name(name), "Expected command or label");
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

		ASSERT_PARS(parse_space_seq(), "Expected space sequence");
		ASSERT_PARS(parse_value(val), "Expected value");
		dynamic_cast<Cmd_PUSH*>(ret)->value = val;
		break;
	case CMD_ID::POP:
		ret = new Cmd_POP();
		break;
	case CMD_ID::PUSHR:
		ret = new Cmd_PUSHR();

		ASSERT_PARS(parse_space_seq(), "Expected space sequence");
		ASSERT_PARS(parse_register(rg), "Expected register name");
		dynamic_cast<Cmd_PUSHR*>(ret)->reg = rg;

		break;
	case CMD_ID::POPR:
		ret = new Cmd_POPR();

		ASSERT_PARS(parse_space_seq(), "Expected space sequence");
		ASSERT_PARS(parse_register(rg), "Expected register name");
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
		name.pop_back();

		status = 0;
		labels_[name] = number;

	} // switch

	if (status == 1)
	{
		std::string lbl;
		ASSERT_PARS(parse_space_seq(), "Expected space sequence");
		ASSERT_PARS(parse_label_name(lbl), "Expected label name");
		jumps_[number] = lbl;
	}
	parse_space_seq();
	ASSERT_PARS(parse_newline_seq(), "Expected a new line after command/label");

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


	for (const auto& [number, name] : jumps_)
	{
		auto search = labels_.find(name);
		if (search != labels_.end())
		{
			dynamic_cast<Cmd_JUMP*>(vec[number])->to = labels_[name];
		}
		else
		{
			std::cout << "Unknown label:" << name << "\n";
			abort();
		}
	}

	return vec;
}
