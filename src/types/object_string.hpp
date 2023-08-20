#pragma once

#include <string>

#include "object.hpp"

namespace cppLox::Types {

/// @brief Represents a string object.
class ObjectString : public Object {
  public:
    /// @brief Constructs a new string object.
    /// @param value The value of the string object.
    ObjectString(std::string const & string) {
        m_string = new std::string(string);
        m_type = Object::Type::STRING;
    }

    ~ObjectString() {
        delete m_string;
    }

    /// @brief Gets the value of the string object.
    /// @return The value of the string object.
    auto string() const -> std::string const & {
        return *m_string;
    }

    virtual void writeToOutputStream(std::ostream & os) const override {
        os << *m_string;
    }

  private:
    /// @brief The value of the underlying string.
    std::string * m_string;
};

} // namespace cppLox::Types