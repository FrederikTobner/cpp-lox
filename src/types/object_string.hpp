#pragma once

#include <memory>
#include <string>

#include "object.hpp"

namespace cppLox::Types {

/// @brief Represents a string object.
class ObjectString : public Object {
  public:
    /// @brief Constructs a new string object.
    /// @param value The value of the string object.
    ObjectString(std::string const & string) {
        m_string = std::make_unique<std::string>(string);
        m_type = Object::Type::STRING;
    }

    ~ObjectString() override = default;

    /// @brief Gets the value of the string object.
    /// @return The value of the string object.
    [[nodiscard]] auto string() const -> std::string const & {
        return *m_string.get();
    }

    virtual void writeToOutputStream(std::ostream & os) const override {
        os << *m_string.get();
    }

  private:
    /// @brief The value of the underlying string.
    std::unique_ptr<std::string> m_string;
};

} // namespace cppLox::Types