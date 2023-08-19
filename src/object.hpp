#pragma once

#include <string>
#include <type_traits>

class Object;

template <typename T>
concept IsDerivedFromObject = std::is_base_of<Object, T>::value;

/// @brief The base class for all objects.
class Object {
  public:
    /// @brief The type of an object.
    enum Type {
        OBJ_STRING
    };
    /// @brief Constructs a new object.
    Object() = default;

    /// @brief Destructor of the object.
    virtual ~Object() = default;

    /// @brief Gets the type of the object.
    /// @return The type of the object.
    Type type() const {
        return m_type;
    }

    /// @brief Checks if the object is of the given type.
    /// @param type The type to check against.
    /// @return True if the object is of the given type, false otherwise.
    bool is(Type type) const {
        return m_type == type;
    }

    template <IsDerivedFromObject T> T * as() {
        return static_cast<T *>(this);
    }

  protected:
    /// @brief The type of the object.
    Type m_type;
};

/// @brief Represents a string object.
class ObjectString : public Object {
  public:
    /// @brief Constructs a new string object.
    /// @param value The value of the string object.
    ObjectString(std::string const & string) : m_string(string) {
        m_type = Object::Type::OBJ_STRING;
    }

    /// @brief Gets the value of the string object.
    /// @return The value of the string object.
    std::string const & string() const {
        return m_string;
    }

  private:
    /// @brief The value of the underlying string.
    std::string m_string;
};