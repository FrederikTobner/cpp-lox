#include "value.hpp"

#include <iostream>

#include "runtime_exception.hpp"

Value::Value() {
    this->m_type = Value::Type::NULL_;
}

Value::Value(bool value) {
    this->m_type = Value::Type::BOOL;
    this->m_underlying_value.m_bool = value;
}

Value::Value(double value) {
    this->m_type = Value::Type::NUMBER;
    this->m_underlying_value.m_number = value;
}

Value::Value(Object * value) {
    this->m_type = Value::Type::OBJECT;
    this->m_underlying_value.m_object = value;
}

[[nodiscard]] auto Value::is(Value::Type type) noexcept -> bool {
    return type == this->m_type;
}

[[nodiscard]] auto Value::getType() const -> Value::Type {
    return this->m_type;
}

auto operator<<(std::ostream & os, Value const & value) -> std::ostream & {
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

[[nodiscard]] auto Value::operator==(Value const & other) const -> bool {
    if (this->m_type != other.m_type) {
        return false;
    }
    switch (this->m_type) {
    case Value::Type::BOOL:
        return this->m_underlying_value.m_bool == other.m_underlying_value.m_bool;
    case Value::Type::NULL_:
        return true;
    case Value::Type::NUMBER:
        return this->m_underlying_value.m_number == other.m_underlying_value.m_number;
    case Value::Type::OBJECT:
        return this->m_underlying_value.m_object == other.m_underlying_value.m_object;
    }
    return false;
}

[[nodiscard]] auto Value::operator!=(Value const & other) const -> bool {
    return !(*this == other);
}

[[nodiscard]] auto Value::operator-() const -> Value {
    if (this->m_type != Value::Type::NUMBER) {
        throw RunTimeException("Runtime error: unary negation is only defined for numbers");
    }
    return Value(-this->m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator!() const -> Value {
    if (this->m_type == Value::Type::BOOL) {
        return Value(!this->m_underlying_value.m_bool);
    } else if (this->m_type == Value::Type::NUMBER) {
        return Value(!this->m_underlying_value.m_number);
    } else if (this->m_type == Value::Type::OBJECT) {
        // Objects are not nullable so invert to false
        return Value(false);
    }
    // Null values are falsy so invert to true
    return Value(true);
}

[[nodiscard]] auto Value::operator+(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("addition is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number + other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator-(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("subtraction is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number - other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator*(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("multiplication is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number * other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator/(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("division is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number / other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator<(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number < other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator<=(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number <= other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator>(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number > other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator>=(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number >= other.m_underlying_value.m_number);
}