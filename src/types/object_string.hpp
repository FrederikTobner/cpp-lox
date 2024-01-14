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

    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << *m_string.get();
    }

    auto operator==(ObjectString const & other) const -> bool {
        return *m_string.get() == other.string();
    }

    auto operator==(ObjectString const * other) const -> bool {
        return *m_string.get() == other->string();
    }

  private:
    /// @brief The value of the underlying string.
    std::unique_ptr<std::string> m_string;
};

} // namespace cppLox::Types