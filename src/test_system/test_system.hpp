#pragma once


#define NORMAL  "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"
#define WHITE   "\x1B[37m"

typedef bool (*TestExec)();

enum TestResult
{
    OK        = 0,
    FAIL      = 1,
    EXCEPTION = 2,
    ERROR     = 3,
    TIMEOUT   = 4
};

constexpr size_t TIMEOUT_MILLISSECONDS = 500U;

TestResult run_test(const char* name, TestExec test, bool show_res=true, size_t time=TIMEOUT_MILLISSECONDS);
