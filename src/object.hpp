#pragma once

#include <format>
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

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend std::ostream & operator<<(std::ostream & os, Object const & dt) {
        dt.print(os);
        return os;
    }
    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend std::ostream & operator<<(std::ostream & os, Object * dt) {
        dt->print(os);
        return os;
    }

    virtual void print(std::ostream & os) const = 0;

  protected:
    /// @brief The type of the object.
    Type m_type;
};
/// @brief Represents a string object.
class ObjectString : public Object {
  public:
    /// @brief Constructs a new string object.
    /// @param value The value of the string object.
    ObjectString(std::string const & string) {
        m_string = new std::string(string);
        m_type = Object::Type::OBJ_STRING;
    }

    ~ObjectString() {
        delete m_string;
    }

    /// @brief Gets the value of the string object.
    /// @return The value of the string object.
    std::string const & string() const {
        return *m_string;
    }

    virtual void print(std::ostream & os) const override {
        os << *m_string;
    }

  private:
    /// @brief The value of the underlying string.
    std::string * m_string;
};

/// @brief Formatter for the Object class.
template <> struct std::formatter<Object *> : std::formatter<std::string> {
    auto format(Object * object, format_context & ctx) const {
        switch (object->type()) {
        case Object::Type::OBJ_STRING:
            return formatter<string>::format(std::format("{}", object->as<ObjectString>()->string()), ctx);
        }
        // should be be unreachable
        return formatter<string>::format(std::format("undefined"), ctx);
    }
};
