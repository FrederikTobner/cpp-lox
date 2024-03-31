/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file value.cpp
 * @brief This file contains the implementation of the Value class.
 */

#include "value.hpp"

#include <iostream>

#include "../error/runtime_exception.hpp"

using namespace cppLox::Types;

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
        throw cppLox::Error::RunTimeException("Runtime error: unary negation is only defined for numbers");
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
        throw cppLox::Error::RunTimeException("addition is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number + other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator-(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("subtraction is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number - other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator*(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("multiplication is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number * other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator/(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("division is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number / other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator<(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number < other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator<=(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number <= other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator>(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number > other.m_underlying_value.m_number);
}

[[nodiscard]] auto Value::operator>=(Value const & other) const -> Value {
    if (this->m_type != Value::Type::NUMBER || other.m_type != Value::Type::NUMBER) {
        throw cppLox::Error::RunTimeException("less than is only defined for numbers");
    }
    return Value(this->m_underlying_value.m_number >= other.m_underlying_value.m_number);
}
