#pragma once

#include <iostream>
#include <type_traits>

namespace cppLox::Types {

// Forward declaration of Object type.
class Object;

/// @brief Checks if the given type is derived from Object.
template <typename T>
concept DerivedFromObject = std::derived_from<T, cppLox::Types::Object>;

/// @brief The base class for all objects.
class Object {
  public:
    /// @brief The type of an object.
    enum class Type {
        /// @brief An ObjectFunction.
        FUNCTION,
        /// @brief An ObjectString.
        STRING
    };

    /// @brief Destructor of the object.
    virtual ~Object() = default;

    /// @brief Gets the type of the object.
    /// @return The type of the object.
    [[nodiscard]] auto type() const -> Object::Type {
        return m_type;
    }

    /// @brief Checks if the object is of the given type.
    /// @param type The type to check against.
    /// @return True if the object is of the given type, false otherwise.
    [[nodiscard]] auto is(Type type) const -> bool {
        return m_type == type;
    }

    /// @brief Casts the object to the given type.
    /// @tparam T The type to cast to.
    /// @return The object cast to the given type.
    template <DerivedFromObject T> [[nodiscard]] auto as() -> T * {
        return static_cast<T *>(this);
    }

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Object const & dt) -> std::ostream & {
        dt.writeToOutputStream(os);
        return os;
    }
    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Object * dt) -> std::ostream & {
        dt->writeToOutputStream(os);
        return os;
    }

    /// @brief Writes the value to the given output stream
    /// @param os The output stream to write to
    virtual void writeToOutputStream(std::ostream & os) const = 0;

  protected:
    /// @brief The type of the object.
    Type m_type;

    /// @brief Constructs a new object.
    Object() = default;
};
} // namespace cppLox::Types
