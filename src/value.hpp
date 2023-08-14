#pragma once

#include <format>
#include <iostream>
#include <string>

/// @brief A value that can be stored in a variable or on the stack
class Value {
  public:
    /// @brief The possible types of a value
    enum Type {
        /// @brief A boolean value
        VAL_BOOL,
        /// @brief A null value
        VAL_NULL,
        /// @brief A numeric value
        VAL_NUMBER,
    };

    /// @brief Construct a new Value object with type VAL_NULL
    Value();

    /// @brief Construct a new Value object with type VAL_BOOL
    Value(bool value);

    /// @brief Construct a new Value object with type VAL_NUMBER
    Value(double value);

    /// @brief Gets the type of the value
    /// @return The type of the value
    [[nodiscard]] Type getType() const;

    /// @brief Checks if the value is of the given type
    /// @param type The type to check against
    /// @return true if the value is of the given type, false otherwise
    [[nodiscard]] bool is(Type type) const noexcept;

    /// @brief Gets the value as a bool
    /// @return The value as a bool
    [[nodiscard]] bool asBool() const;

    /// @brief Gets the value as a number
    /// @return The value as a number
    [[nodiscard]] double asNumber() const;

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend std::ostream & operator<<(std::ostream & os, Value const & dt);

    /// @brief Compares two values
    /// @param other The value to compare against
    /// @return true if the values are equal, false otherwise
    [[nodiscard]] bool operator==(Value const & other) const;

    /// @brief Compares two values
    /// @param other The value to compare against
    /// @return true if the values are not equal, false otherwise
    [[nodiscard]] bool operator!=(Value const & other) const;

    /// @brief Adds two values
    /// @param other The value to add
    /// @return The sum of the two values
    [[nodiscard]] Value operator+(Value const & other) const;

    /// @brief Subtracts two values
    /// @param other The value to subtract
    /// @return The difference of the two values
    [[nodiscard]] Value operator-(Value const & other) const;

    /// @brief Multiplies two values
    /// @param other The value to multiply
    /// @return The product of the two values
    [[nodiscard]] Value operator*(Value const & other) const;

    /// @brief Divides two values
    /// @param other The value to divide
    /// @return The quotient of the two values
    [[nodiscard]] Value operator/(Value const & other) const;

    /// @brief Determines if one value is less than another
    /// @param other The value to compare against
    /// @return true if the first value is less than the second, false otherwise
    [[nodiscard]] Value operator<(Value const & other) const;

    /// @brief Determines if one value is less than or equal to another
    /// @param other The value to compare against
    /// @return true if the first value is less than or equal to the second, false otherwise
    [[nodiscard]] Value operator<=(Value const & other) const;

    /// @brief Determines if one value is greater than or equal to another
    /// @param other The value to compare against
    /// @return true if the first value is greater than or equal to the second, false otherwise
    [[nodiscard]] Value operator>(Value const & other) const;

    /// @brief Determines if one value is greater than another
    /// @param other The value to compare against
    /// @return true if the first value is greater than the second, false otherwise
    [[nodiscard]] Value operator>=(Value const & other) const;

    /// @brief Negates the value
    /// @return The negated value
    [[nodiscard]] Value operator-() const;

    /// @brief Inverts a boolean value
    /// @return The inverted value
    [[nodiscard]] Value operator!() const;

  private:
    /// @brief The type of the value
    Type m_type;

    /// @brief The underlying value
    union {
        /// @brief The underlying bool value
        bool m_bool;
        /// @brief The underlying number value
        double m_number;
    } m_underlying_value;
};
template <> struct std::formatter<Value> : std::formatter<std::string_view> {
    template <typename Context> auto format(Value val, Context & ctx) {
        switch (val.getType()) {
        case Value::VAL_BOOL:
            return std::formatter<std::string_view>::format(std::format("{}", val.asBool()), ctx);
        case Value::VAL_NULL:
            return std::formatter<std::string_view>::format("null", ctx);
        case Value::VAL_NUMBER:
            return std::formatter<std::string_view>::format(std::format("{}", val.asNumber()), ctx);
        }
        return ctx.out();
    }
};