#pragma once

#include <exception>
#include <string>

class RunTimeException : public std::exception {
  public:
    RunTimeException(std::string message) : m_message(message) {
    }
    virtual auto what() const throw() -> char const * {
        return m_message.c_str();
    }

  private:
    std::string m_message;
};