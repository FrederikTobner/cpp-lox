#pragma once

#include <iostream>
#include <type_traits>

class Object;

template <typename T>
concept IsDerivedFromObject = std::is_base_of<Object, T>::value;

/// @brief The base class for all objects.
class Object {
  public:
    /// @brief The type of an object.
    enum class Type {
        STRING
    };
    /// @brief Constructs a new object.
    Object() = default;

    /// @brief Destructor of the object.
    virtual ~Object() = default;

    /// @brief Gets the type of the object.
    /// @return The type of the object.
    auto type() const -> Object::Type {
        return m_type;
    }

    /// @brief Checks if the object is of the given type.
    /// @param type The type to check against.
    /// @return True if the object is of the given type, false otherwise.
    auto is(Type type) const -> bool {
        return m_type == type;
    }

    template <IsDerivedFromObject T> auto as() -> T * {
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

    virtual void writeToOutputStream(std::ostream & os) const = 0;

  protected:
    /// @brief The type of the object.
    Type m_type;
};
