/**
 * @file runtime_exception.cpp
 * @brief This file contains the implementation of the RunTimeException class.
 */

#include "runtime_exception.hpp"

using namespace cppLox::Error;

RunTimeException::RunTimeException(std::string_view message) : m_message(message) {
}

[[nodiscard]] char const * RunTimeException::what() const noexcept {
    return m_message.c_str();
}