#pragma once

#include <iostream>
#include <type_traits>

#include "object.hpp"
#include "object_formatter.hpp"

namespace cppLox::Types {

template <typename T>
concept IsAnUnderLyingValueType = std::is_same_v<T, bool> || std::is_same_v<T, double> || std::is_same_v<T, Object *>;

/// @brief A value that can be stored in a variable or on the stack
class Value {
  public:
    /// @brief The possible types of a value
    enum class Type {
        /// @brief A boolean value
        BOOL,
        /// @brief A null value
        NULL_,
        /// @brief A numeric value
        NUMBER,
        /// @brief An object value
        OBJECT
    };

    /// @brief Construct a new Value object with type VAL_NULL
    Value();

    /// @brief Construct a new Value object with type VAL_BOOL
    Value(bool value);

    /// @brief Construct a new Value object with type VAL_NUMBER
    Value(double value);

    /// @brief Construct a new Value object with type VAL_OBJECT
    Value(Object * value);

    /// @brief Destroy a Value object
    ~Value() = default;

    /// @brief Gets the type of the value
    /// @return The type of the value
    [[nodiscard]] auto getType() const -> Value::Type;

    /// @brief Checks if the value is of the given type
    /// @param type The type to check against
    /// @return true if the value is of the given type, false otherwise
    [[nodiscard]] auto is(Type type) noexcept -> bool;

    /// @brief Gets the underlying value as the given type
    /// @tparam T The type to get the underlying value as
    /// @return The underlying value as the given type
    template <IsAnUnderLyingValueType T> [[nodiscard]] auto as() -> T {
        if constexpr (std::is_same_v<T, bool>) {
            return m_underlying_value.m_bool;
        } else if constexpr (std::is_same_v<T, double>) {
            return m_underlying_value.m_number;
        } else if constexpr (std::is_same_v<T, Object *>) {
            return m_underlying_value.m_object;
        }
    }

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Value const & value) -> std::ostream & {
        switch (value.m_type) {
        case Value::Type::BOOL:
            return os << (value.m_underlying_value.m_bool ? "true" : "false");
        case Value::Type::NULL_:
            return os << "null";
        case Value::Type::NUMBER:
            {
                // Remove trailing zeros and decimal point if there are no fractional digits
                std::string str = std::to_string(value.m_underlying_value.m_number);
                str.erase(str.find_last_not_of('0') + 1, std::string::npos);
                str.erase(str.find_last_not_of('.') + 1, std::string::npos);
                return os << str;
            }
        case Value::Type::OBJECT:
            return os << value.m_underlying_value.m_object;
        }
        // should be be unreachable
        return os << "undefined";
    }

    /// @brief Compares two values
    /// @param other The value to compare against
    /// @return true if the values are equal, false otherwise
    [[nodiscard]] auto operator==(Value const & other) const -> bool;

    /// @brief Compares two values
    /// @param other The value to compare against
    /// @return true if the values are not equal, false otherwise
    [[nodiscard]] auto operator!=(Value const & other) const -> bool;

    /// @brief Adds two values
    /// @param other The value to add
    /// @return The sum of the two values
    [[nodiscard]] auto operator+(Value const & other) const -> Value;

    /// @brief Subtracts two values
    /// @param other The value to subtract
    /// @return The difference of the two values
    [[nodiscard]] auto operator-(Value const & other) const -> Value;

    /// @brief Multiplies two values
    /// @param other The value to multiply
    /// @return The product of the two values
    [[nodiscard]] auto operator*(Value const & other) const -> Value;

    /// @brief Divides two values
    /// @param other The value to divide
    /// @return The quotient of the two values
    [[nodiscard]] auto operator/(Value const & other) const -> Value;

    /// @brief Determines if one value is less than another
    /// @param other The value to compare against
    /// @return true if the first value is less than the second, false otherwise
    [[nodiscard]] auto operator<(Value const & other) const -> Value;

    /// @brief Determines if one value is less than or equal to another
    /// @param other The value to compare against
    /// @return true if the first value is less than or equal to the second, false otherwise
    [[nodiscard]] auto operator<=(Value const & other) const -> Value;

    /// @brief Determines if one value is greater than or equal to another
    /// @param other The value to compare against
    /// @return true if the first value is greater than or equal to the second, false otherwise
    [[nodiscard]] auto operator>(Value const & other) const -> Value;

    /// @brief Determines if one value is greater than another
    /// @param other The value to compare against
    /// @return true if the first value is greater than the second, false otherwise
    [[nodiscard]] auto operator>=(Value const & other) const -> Value;

    /// @brief Negates the value
    /// @return The negated value
    [[nodiscard]] auto operator-() const -> Value;

    /// @brief Inverts a boolean value
    /// @return The inverted value
    [[nodiscard]] auto operator!() const -> Value;

  private:
    /// @brief The type of the value
    Type m_type;

    /// @brief The underlying value
    union {
        /// @brief The underlying bool value
        bool m_bool;
        /// @brief The underlying number value
        double m_number;
        /// @brief The underlying object value
        Object * m_object;
    } m_underlying_value;
};
} // namespace cppLox::Types