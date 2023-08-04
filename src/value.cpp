#include "value.hpp"

#include <iostream>

Value::Value() {
    this->m_type = VAL_NIL;
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

bool Value::isBool() const {
    return this->m_type == VAL_BOOL;
}

bool Value::isNil() const {
    return this->m_type == VAL_NIL;
}

bool Value::isNumber() const {
    return this->m_type == VAL_NUMBER;
}

bool Value::asBool() const {
    return this->m_underlying_value.m_bool;
}

double Value::asNumber() const {
    return this->m_underlying_value.m_number;
}

void Value::print() const {
    switch (this->m_type) {
    case VAL_BOOL:
        std::cout << (this->asBool() ? "true" : "false");
        break;
    case VAL_NIL:
        std::cout << "nil";
        break;
    case VAL_NUMBER:
        std::cout << this->asNumber();
        break;
    }
}

Value::Type Value::getType() const {
    return this->m_type;
}
