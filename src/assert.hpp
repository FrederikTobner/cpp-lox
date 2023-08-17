#pragma once

#include <format>
#include <iostream>

#ifndef NDEBUG
#define ASSERT(condition, message)                                                                             \
    do {                                                                                                       \
        if (!(condition)) {                                                                                    \
            std::cerr << std::format("Assertion '{}' failed in {} line {}: {}", condition, __FILE__, __LINE__, \
                                     message)                                                                  \
                      << std::endl;                                                                            \
            std::terminate();                                                                                  \
        }                                                                                                      \
    } while (false)
#else
#define ASSERT(condition, message) \
    do {                           \
    } while (false)
#endif
