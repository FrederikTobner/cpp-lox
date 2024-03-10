#pragma once

#include "../types/object_function.hpp"
#include "../types/value.hpp"

namespace cppLox::Backend {
class VM;
class CallFrame {
    friend class VM;

  public:
    CallFrame() = default;

    CallFrame(cppLox::Types::ObjectFunction * function, cppLox::Types::Value * slots);

    [[nodiscard]] auto function() const noexcept -> cppLox::Types::ObjectFunction *;

    [[nodiscard]] auto instructionPointer() const noexcept -> uint8_t *;

    [[nodiscard]] auto slots() const noexcept -> cppLox::Types::Value *;

  private:
    cppLox::Types::ObjectFunction * m_function;
    uint8_t * m_instruction_pointer;
    cppLox::Types::Value * m_slots;
};
} // namespace cppLox::Backend