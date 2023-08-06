#include "value.hpp"

#include <iostream>

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

Value::~Value() {
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

[[nodiscard]] Value::Type Value::getType() const {
    return this->m_type;
}

std::ostream & operator<<(std::ostream & os, const Value & value) {
    switch (value.m_type) {
    case Value::VAL_BOOL:
        os << (value.asBool() ? "true" : "false");
        break;
    case Value::VAL_NULL:
        os << "nil";
        break;
    case Value::VAL_NUMBER:
        os << value.asNumber();
        break;
    }
    return os;
}

[[nodiscard]] bool Value::operator==(const Value & other) const {
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

[[nodiscard]] bool Value::operator!=(const Value & other) const {
    return !(*this == other);
}

[[nodiscard]] Value Value::operator-() const {
    if (this->m_type != VAL_NUMBER) {
        std::cerr << "Runtime error: unary negation is only defined for numbers" << std::endl;
        exit(1);
    }
    return Value(-this->m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator+(const Value & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        std::cerr << "Runtime error: addition is only defined for numbers" << std::endl;
        exit(1);
    }
    return Value(this->m_underlying_value.m_number + other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator-(const Value & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        std::cerr << "Runtime error: subtraction is only defined for numbers" << std::endl;
        exit(1);
    }
    return Value(this->m_underlying_value.m_number - other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator*(const Value & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        std::cerr << "Runtime error: multiplication is only defined for numbers" << std::endl;
        exit(1);
    }
    return Value(this->m_underlying_value.m_number * other.m_underlying_value.m_number);
}

[[nodiscard]] Value Value::operator/(const Value & other) const {
    if (this->m_type != VAL_NUMBER || other.m_type != VAL_NUMBER) {
        std::cerr << "Runtime error: division is only defined for numbers" << std::endl;
        exit(1);
    }
    return Value(this->m_underlying_value.m_number / other.m_underlying_value.m_number);
}