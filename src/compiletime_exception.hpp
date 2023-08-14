#pragma once

#include <exception>
#include <string>

class CompileTimeException : public std::exception {
  public:
    CompileTimeException(std::string message) : m_message(message) {
    }
    virtual char const * what() const throw() {
        return m_message.c_str();
    }

  private:
    std::string m_message;
};