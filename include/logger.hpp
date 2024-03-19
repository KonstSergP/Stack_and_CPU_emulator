#pragma once
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <vector>
#include <cstring>

struct Log_store
{

	std::vector<int> lines_ = {0, 0, 0, 0, 0, 0, 0};

	std::vector<std::string> log_table_ = 
	{
	"Constructor from value",
	"Constructor from rvalue ref on value",
	"Constructor from other",
	"Constructor from rvalue ref on other",
	"Assignment from const &",
	"Assignment from rvalue ref",
	"Destructor"
    };

    void print_info()
	{
		std::cout << "\n";
		for (size_t i = 0; i < lines_.size(); i++)
		{
			if (lines_[i] != 0)
			{
				std::cout << log_table_[i] << " " << lines_[i] << "\n";
			}
		}
	}

	void save_line(int val)
	{
		lines_[val]++;
	}

    ~Log_store()
    {
    	print_info();
    }
} Logger_storage;



template <typename Data_t>
class Logger
{
public:

	Logger() = default;
	Logger(const Data_t& element);
	Logger(Data_t&& element);
	Logger(const Logger& other);
	Logger(Logger&& other);

	Logger& operator=(const Logger& other);
    Logger& operator=(Logger&& other);

    ~Logger();

    bool operator==(const Logger& other) const;
    bool operator!=(const Logger& other) const;


private:
	Data_t element_;

};

template <typename Data_t>
Logger<Data_t>::Logger(const Data_t& element) : element_(element)
{
	Logger_storage.save_line(0);
}

template <typename Data_t>
Logger<Data_t>::Logger(Data_t&& element) : element_(std::forward<Data_t>(element))
{
	Logger_storage.save_line(1);
}

template <typename Data_t>
Logger<Data_t>::Logger(const Logger<Data_t>& other)
{
	element_ = other.element_;
	Logger_storage.save_line(2);
}

template <typename Data_t>
Logger<Data_t>::Logger(Logger<Data_t>&& other)
{
	element_ = std::move(other.element_);
	Logger_storage.save_line(3);
}

template <typename Data_t>
Logger<Data_t>& Logger<Data_t>::operator=(const Logger<Data_t>& other)
{
    element_.~Data_t();

    element_ = other.element_;

    Logger_storage.save_line(4);

    return *this;
}

template <typename Data_t>
Logger<Data_t>& Logger<Data_t>::operator=(Logger<Data_t>&& other)
{
    element_.~Data_t();

    element_ = std::move(other.element_);

    Logger_storage.save_line(5);

    return *this;
}

template <typename Data_t>
Logger<Data_t>::~Logger()
{
	element_.~Data_t();

	Logger_storage.save_line(6);
}

template <typename Data_t>
bool Logger<Data_t>::operator==(const Logger<Data_t>& other) const
{
    return element_ == other.element_;
}

template <typename Data_t>
bool Logger<Data_t>::operator!=(const Logger<Data_t>& other) const
{
    return element_ != other.element_;
}
