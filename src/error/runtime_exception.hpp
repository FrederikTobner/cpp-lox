#pragma once

#include <exception>
#include <string>

namespace cppLox::Error {

/// @brief Exception that is thrown when an error occurs during run time.
class RunTimeException : public std::exception {
  public:
    RunTimeException(std::string_view message) : m_message(message) {
    }
    [[nodiscard]] virtual auto what() const throw() -> char const * {
        return m_message.c_str();
    }

  private:
    std::string m_message;
};

} // namespace cppLox::Error