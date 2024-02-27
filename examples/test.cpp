#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "../include/myStack.hpp"
#include "../src/test_system/test_system.hpp"

#define CHECK(x) if (not (x)) {return FAIL;}
constexpr size_t SIZE = 100U;

bool test_ok()
{
	return OK;
}

bool test_fail()
{
	return FAIL;
}

bool test_timeout()
{
	long long int cnt = 0;
	while (true) {cnt++;}
}

bool test_exception()
{
	throw std::bad_exception();
}

bool test_error()
{
	abort();
}

bool test_testsystem()
{
	CHECK(run_test("test_ok", test_ok, 0) == OK);
	CHECK(run_test("test_fail", test_fail, 0) == FAIL);
	CHECK(run_test("test_timeout", test_timeout, 0) == TIMEOUT);
	CHECK(run_test("test_exception", test_exception, 0) == EXCEPTION);
	CHECK(run_test("test_error", test_error, 0) == ERROR);
	return OK;
}


using namespace handMadeStack;

template <typename Data_t>
void fill_stack(myStack<Data_t>& stack)
{
    for (size_t i = 0U; i < SIZE; ++i)
    {
        stack.push(i);
    }
}

template <typename Data_t>
bool test_constructor()
{
	try
    {
        myStack<Data_t> stack1(0);
        myStack<Data_t> stack2(SIZE);
    }
    catch (const std::exception& exc)
    {
        return FAIL;
    }

    return OK;
}

template <typename Data_t>
bool test_copy_constructor()
{

    myStack<Data_t> stack1{SIZE};
    fill_stack(stack1);

    myStack<Data_t> stack2{stack1};

    return (stack1 == stack2)? OK : FAIL;
}

template <typename Data_t>
bool test_move_constructor()
{

    myStack<Data_t> stack1{SIZE};
    myStack<Data_t> stack2{SIZE};

    fill_stack(stack1);
    fill_stack(stack2);

    myStack<Data_t> stack3{std::move(stack1)};

    return (stack2 == stack3)? OK : FAIL;
}

template <typename Data_t>
bool test_copy_assignment()
{

    myStack<Data_t> stack1{SIZE};
    fill_stack(stack1);

    myStack<Data_t> stack2{2*SIZE};

    stack2 = stack1;

    return (stack1 == stack2) ? OK : FAIL;
}

template <typename Data_t>
bool test_move_assignment()
{

    myStack<Data_t> stack1{SIZE};
    myStack<Data_t> stack2{SIZE};

    fill_stack(stack1);
    fill_stack(stack2);

    myStack<Data_t> stack3{2*SIZE};

    stack3 = std::move(stack1);

    return (stack2 == stack3)? OK : FAIL;
}


int main()
{	
	run_test("Testing system", test_testsystem, true, 2000);
    printf("Testing myStack<int>:\n");
    run_test("int-constructor",      test_constructor<int>);
    run_test("int-copy-constructor", test_copy_constructor<int>);
    run_test("int-move-constructor", test_move_constructor<int>);
    run_test("int-copy-assignment",  test_copy_assignment<int>);
    run_test("int-move-assignment",  test_move_assignment<int>);

    printf("\n");

	return 0;
}