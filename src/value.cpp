#include "value.hpp"

#include <iostream>

#include "runtime_exception.hpp"

Value::Value() {
    this->m_type = VAL_NULL;
}

Value::Value(bool value) {
    this->m_type = VAL_BOOL;
    this->m_underlying_value.m_bool = value;
}

Value::Value(double value) {
    this->m_type = VAL_NUMBER;
    this->m_underlying_value.m_number = value;
}

[[nodiscard]] bool Value::is(Value::Type type) const noexcept {
    return type == this->m_type;
}

[[nodiscard]] bool Value::asBool() const {
    return this->m_underlying_value.m_bool;
}

[[nodiscard]] double Value::asNumber() const {
    return this->m_underlying_value.m_number;
}

[[nodiscard]] std::string Value::asString() const {
    switch (this->m_type) {
    case VAL_BOOL:
        return this->m_underlying_value.m_bool ? "true" : "false";
    case VAL_NULL:
        return "null";
    case VAL_NUMBER:
        {
            // Remove trailing zeros and decimal point if there are no fractional digits
            std::string str = std::to_string(this->m_underlying_value.m_number);
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            return str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        }
    }
    return "";
}

[[nodiscard]] Value::Type Value::getType() const {
    return this->m_type;
}

std::ostream & operator<<(std::ostream & os, Value const & value) {
    return os << value.asString();
}

[[nodiscard]] bool Value::operator==(Value const & other) const {
    if (this->m_type != other.m_type) {
        return false;
    }
    switch (this->m_type) {
    case VAL_BOOL:
        return this->m_underlying_value.m_bool == other.m_underlying_value.m_bool;
    case VAL_NULL:
        return true;
    case VAL_NUMBER:
        return this->m_underlying_value.m_number == other.m_underlying_value.m_number;
    }
    return false;
}

[[nodiscard]] bool Value::operator!=(Value const & other) const {
    return !(*this == other);
}

[[nodiscard]] Value Value::operator-() const {
    if (this->m_type != VAL_NUMBER) {
        throw RunTimeException("Runtime error: unary negation is only defined for numbers");
    }
    return Value(-this->m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator!() const {
    if (this->m_type != VAL_BOOL) {
        throw RunTimeException("Runtime error: Unary not is only defined for booleans");
    }
    return Value(!this->m_underlying_value.m_bool);
}

[[nodiscard]] Value Value::operator+(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("addition is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number + other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator-(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("subtraction is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number - other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator*(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("multiplication is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number * other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator/(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("division is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number / other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator<(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number < other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator<=(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number <= other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator>(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number > other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator>=(Value const & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        throw RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number >= other.m_underlying_value.m_number);
}