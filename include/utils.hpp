#pragma once

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <cstring>


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


// Have non-trivial copy constructors
//template <typename Data_t, class = typename std::enable_if<!std::is_trivially_copyable<Data_t>::value>::type>
template <typename Data_t>
typename std::enable_if<!std::is_trivially_copyable<Data_t>::value>::type
my_copy_n(Data_t* src, size_t sz, Data_t* dst)
{
    for (size_t i = 0; i < sz; i++)
    {
        dst[i] = src[i];
    }
}

// Have trivial copy constructors
template <typename Data_t>
typename std::enable_if<std::is_trivially_copyable<Data_t>::value>::type
my_copy_n(Data_t* src, size_t sz, Data_t* dst)
{
    std::memcpy(dst, src, sizeof(Data_t) * sz);
}
