#include "object_upvalue.hpp"

using namespace cppLox::Types;

ObjectUpValue::ObjectUpValue(Value * closed) : m_closed(closed) {
    m_type = Object::Type::UPVALUE;
}

auto ObjectUpValue::closed() const noexcept -> Value * {
    return m_closed;
}

auto ObjectUpValue::setClosed(Value * closed) -> void {
    m_closed = closed;
}