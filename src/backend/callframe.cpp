#include "callframe.hpp"

using namespace cppLox::Backend;

CallFrame::CallFrame(cppLox::Types::ObjectFunction * function, cppLox::Types::Value * slots)
    : m_function(function), m_slots(slots) {
    m_instruction_pointer = function->chunk()->code().data();
}

[[nodiscard]] auto CallFrame::function() const noexcept -> cppLox::Types::ObjectFunction * {
    return m_function;
}

[[nodiscard]] auto CallFrame::instructionPointer() const noexcept -> uint8_t * {
    return m_instruction_pointer;
}

[[nodiscard]] auto CallFrame::slots() const noexcept -> cppLox::Types::Value * {
    return m_slots;
}