#include <cstdio>
#include <stdlib.h>
#include <exception>
#include "test_system.hpp"

// Specific for linux
#include <unistd.h>
#include <sys/wait.h>

TestResult run_test(const char* name, TestExec test, bool show_res, size_t time)
{
	if (show_res) {printf("Test %20s: ", name);}
	fflush(stdout);

    // Create child process with same data and code
	pid_t child_pid = fork();
    if (child_pid == -1)
    {
        fprintf(stderr, "Unable to create child process!\n");
        exit(EXIT_FAILURE);
    }


    // child_pid == 0 => It is child process

    if (child_pid == 0)
    {
        try 
        {
            bool result = test();
            exit(result);
        }
        catch (const std::exception& exc)
        {
            if (show_res) {printf("%s[EXC] (exception: %s)\n%s", MAGENTA, NORMAL, exc.what());}

            exit(EXCEPTION);
        }
    }

    // child_pid != 0 => It is parent process

    pid_t return_value = 0;
    int status = 0;
    for (size_t tm = 0U; tm < time; tm++)
    {
        // Wait for child process
        // With WNOHANG function will check if child exited and then return
        return_value = waitpid(child_pid, &status, WNOHANG);

        // return_value == -1 => Error in waitpid()
        if (return_value == -1)
        {
            fprintf(stderr, "Unable to call wait()\n");
            exit(EXIT_FAILURE);
        }

        // Exit loop
        if (return_value != 0) {break;}

        // Sleep for 1 millisecond:
        usleep(1000U);
    }

    // If it was timeout
    if (return_value == 0)
    {   
        kill(child_pid, SIGKILL);
        if (show_res) {printf("%s[TIMEOUT]\n%s", MAGENTA, NORMAL);}
        return TIMEOUT;
    }

    // WIFEXITED   - If exit was correct
    // WEXITSTATUS - Returned value
    // WIFSIGNALED - If child was stopped with a signal
    // WTERMSIG    - Returns signal number

    // If exit was correct
    if (WIFEXITED(status))
    {
        switch (WEXITSTATUS(status))
        {
            case OK:
            {
                if (show_res) {printf("%s[OK]\n%s", GREEN, NORMAL);}
                return OK;
            }
            case FAIL:
            {
                if (show_res) {printf("%s[FAIL]\n%s", RED, NORMAL);}
                return FAIL;
            }
            case EXCEPTION:
            {
                if (show_res) {printf("%s[EXC]\n%s", MAGENTA, NORMAL);}
                return EXCEPTION;
            }
            default:
            {
                fprintf(stderr, "Unexpected switch case\n");
                exit(EXIT_FAILURE);
            }
        };
    }
    // Handle error exit:
    else if (WIFSIGNALED(status))
    {
        if (show_res) {printf("%s[ERROR] (killed by signal %d)\n%s", MAGENTA, WTERMSIG(status), NORMAL);}
        return ERROR;
    }

    // This point must be unreachable:
    fprintf(stderr, "Unexpected result of waitpid()\n");
    exit(EXIT_FAILURE);
}
