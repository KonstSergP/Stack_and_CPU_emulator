#pragma once

#include <cstdlib>
#include <cstdio>
#include <stdexcept>


constexpr size_t EXCEPTION_STR_SIZE = 150U;
extern char explanation_str[EXCEPTION_STR_SIZE];


#ifndef NDEBUG
#define ASSERT(contract, format, ...)         \
    do {                                               \
        if (!(contract)) {                             \
            snprintf(                                  \
                explanation_str, EXCEPTION_STR_SIZE,   \
                "[%s:%d:%s] " format "\n",             \
                __FILE__, __LINE__, __func__,          \
                ##__VA_ARGS__);                        \
                                                       \
            throw std::runtime_error(explanation_str); \
        }                                              \
    } while (0)
#else
#define ASSERT(contract, format, ...) \
    do {} while (0)
#endif // NDEBUG