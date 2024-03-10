#include "vm.hpp"

#include <iostream>

#include "../bytecode/opcode.hpp"
#include "../error/runtime_exception.hpp"
#include "../types/object_formatter.hpp"
#include "../types/object_string.hpp"
#include "../types/value_formatter.hpp"

using namespace cppLox::Backend;

VM::VM(std::shared_ptr<cppLox::MemoryMutator> memoryMutator) {
    m_stack_top = 0;
    m_frame_count = 0;
    m_memoryMutator = memoryMutator;
}

auto VM::interpret(cppLox::Types::ObjectFunction & function) -> void {
    CallFrame * frame = &m_frames[m_frame_count++];
    frame->m_function = &function;
    frame->m_instruction_pointer = function.chunk()->code().data();
    frame->m_slots = m_stack + 1;
    push(*frame, cppLox::Types::Value(&function));
    run(function);
    return;
}

auto VM::run(cppLox::Types::ObjectFunction & function) -> void {
    CallFrame * frame = &m_frames[m_frame_count - 1];
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (cppLox::Types::Value * slot = m_stack; (size_t)slot - m_stack < m_stack_top; slot++) {
            std::cout << std::format("[ {} ]", *slot);
        }
        std::cout << std::endl;
        frame->m_function->chunk()->disassembleInstruction((size_t)frame->m_instruction_index -
                                                           frame->m_function->chunk()->code().data());
#endif
        uint8_t const instruction = *frame->m_instruction_pointer++;
        switch (static_cast<cppLox::ByteCode::Opcode>(instruction)) {
        case cppLox::ByteCode::Opcode::ADD:
            {
                cppLox::Types::Value a = pop(*frame);
                cppLox::Types::Value b = pop(*frame);
                if (a.is(cppLox::Types::Value::Type::NUMBER) && b.is(cppLox::Types::Value::Type::NUMBER)) {
                    push(*frame, a + b);
                } else if (a.is(cppLox::Types::Value::Type::OBJECT) && b.is(cppLox::Types::Value::Type::OBJECT)) {
                    cppLox::Types::Object * aObject = a.as<cppLox::Types::Object *>();
                    cppLox::Types::Object * bObject = b.as<cppLox::Types::Object *>();
                    if (aObject->is(cppLox::Types::Object::Type::STRING) &&
                        bObject->is(cppLox::Types::Object::Type::STRING)) {
                        push(*frame, cppLox::Types::Value(m_memoryMutator->create<cppLox::Types::ObjectString>(
                                         bObject->as<cppLox::Types::ObjectString>()->string() +
                                         aObject->as<cppLox::Types::ObjectString>()->string())));
                    } else {
                        throw cppLox::Error::RunTimeException("Operands must be two numbers or two strings");
                    }
                } else {
                    throw cppLox::Error::RunTimeException("Operands must be two numbers or two strings");
                }
                break;
            }
        case cppLox::ByteCode::Opcode::CONSTANT:
            {
                uint8_t const constant = *frame->m_instruction_pointer++;
                push(*frame, function.chunk()->getConstant(constant));
                break;
            }
        case cppLox::ByteCode::Opcode::DEFINE_GLOBAL:
            {
                uint8_t const constant = *frame->m_instruction_pointer++;
                cppLox::Types::Value const value = pop(*frame);
                if (m_memoryMutator->setGlobal(function.chunk()
                                                   ->getConstant(constant)
                                                   .as<cppLox::Types::Object *>()
                                                   ->as<cppLox::Types::ObjectString>(),
                                               value)) {
                    throw cppLox::Error::RunTimeException("Variable already defined");
                }
                push(*frame, value);
                break;
            }
        case cppLox::ByteCode::Opcode::DIVIDE:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b / a);
                break;
            }
        case cppLox::ByteCode::Opcode::EQUAL:
            push(*frame, pop(*frame) == pop(*frame));
            break;
        case cppLox::ByteCode::Opcode::FALSE:
            push(*frame, cppLox::Types::Value(false));
            break;
        case cppLox::ByteCode::Opcode::GET_GLOBAL:
            {
                uint8_t const constant = *frame->m_instruction_pointer++;
                cppLox::Types::Value const value = m_memoryMutator->getGlobal(frame->m_function->chunk()
                                                                                  ->getConstant(constant)
                                                                                  .as<cppLox::Types::Object *>()
                                                                                  ->as<cppLox::Types::ObjectString>());
                push(*frame, value);
                break;
            }
        case cppLox::ByteCode::Opcode::GET_LOCAL:
            {
                uint8_t const slot = *frame->m_instruction_pointer++;
                push(*frame, frame->m_slots[slot]);
                break;
            }
        case cppLox::ByteCode::Opcode::GREATER:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b > a);
                break;
            }
        case cppLox::ByteCode::Opcode::GREATER_EQUAL:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b >= a);
                break;
            }
        case cppLox::ByteCode::Opcode::JUMP:
            {
                uint16_t const offset = getShort(*frame);
                frame->m_instruction_pointer += offset;
                break;
            }
        case cppLox::ByteCode::Opcode::JUMP_IF_FALSE:
            {
                uint16_t const offset = getShort(*frame);
                if (!peek(*frame).as<bool>()) {
                    frame->m_instruction_pointer += offset;
                }
                break;
            }
        case cppLox::ByteCode::Opcode::LESS:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b < a);
                break;
            }
        case cppLox::ByteCode::Opcode::LESS_EQUAL:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b <= a);
                break;
            }
        case cppLox::ByteCode::Opcode::LOOP:
            {
                uint16_t const offset = getShort(*frame);
                frame->m_instruction_pointer -= offset;
                break;
            }
        case cppLox::ByteCode::Opcode::MULTIPLY:
            push(*frame, pop(*frame) * pop(*frame));
            break;
        case cppLox::ByteCode::Opcode::NEGATE:
            push(*frame, -pop(*frame));
            break;
        case cppLox::ByteCode::Opcode::NOT:
            push(*frame, !pop(*frame));
            break;
        case cppLox::ByteCode::Opcode::NOT_EQUAL:
            push(*frame, pop(*frame) != pop(*frame));
            break;
        case cppLox::ByteCode::Opcode::NULL_:
            push(*frame, cppLox::Types::Value());
            break;
        case cppLox::ByteCode::Opcode::RETURN:
            return;
        case cppLox::ByteCode::Opcode::PRINT:
            std::cout << pop(*frame) << std::endl;
            break;
        case cppLox::ByteCode::Opcode::SET_GLOBAL:
            {
                uint8_t const constant = *frame->m_instruction_pointer++;
                cppLox::Types::Value value = pop(*frame);
                cppLox::Types::ObjectString * name = function.chunk()
                                                         ->getConstant(constant)
                                                         .as<cppLox::Types::Object *>()
                                                         ->as<cppLox::Types::ObjectString>();
                if (!m_memoryMutator->setGlobal(name, value)) {
                    m_memoryMutator->deleteGlobal(name);
                    runTimeError(*frame, "Undefined variable '%s'", name->string().c_str());
                };
                break;
            }
        case cppLox::ByteCode::Opcode::SET_LOCAL:
            {
                uint8_t const slot = *frame->m_instruction_pointer++;
                frame->m_slots[slot] = peek(*frame);
                break;
            }
        case cppLox::ByteCode::Opcode::SUBTRACT:
            {
                cppLox::Types::Value const a = pop(*frame);
                cppLox::Types::Value const b = pop(*frame);
                push(*frame, b - a);
                break;
            }
        case cppLox::ByteCode::Opcode::TRUE:
            push(*frame, cppLox::Types::Value(true));
            break;
        }
    }
}

auto VM::push(CallFrame & frame, cppLox::Types::Value value) -> void {
    if (m_stack_top == STACK_MAX) {
        runTimeError(frame, "Stack overflow");
    }
    m_stack[m_stack_top] = value;
    m_stack_top++;
}

[[nodiscard]] auto VM::pop(CallFrame & frame) -> cppLox::Types::Value {
    if (m_stack_top == 0) {
        runTimeError(frame, "Stack empty on pop");
    }
    m_stack_top--;
    return m_stack[m_stack_top];
}

[[nodiscard]] auto VM::peek(CallFrame & frame) -> cppLox::Types::Value {
    if (m_stack_top == 0) {
        runTimeError(frame, "Stack empty on peek");
    }
    return m_stack[m_stack_top - 1];
}

auto VM::resetStack() noexcept -> void {
    m_stack_top = 0;
    m_frame_count = 0;
}

[[nodiscard]] auto VM::getShort(CallFrame & frame) -> uint16_t {
    uint16_t const short_value =
        static_cast<uint16_t>(*frame.m_instruction_pointer << 8 | *(frame.m_instruction_pointer + 1));
    frame.m_instruction_pointer += 2;
    return short_value;
}

template <class... Args> auto VM::runTimeError(CallFrame & frame, std::string_view fmt, Args &&... args) -> void {
    std::string const errorMessage = std::vformat(fmt, std::make_format_args(args...));
    size_t const index = frame.m_instruction_pointer - frame.m_function->chunk()->code().data();
    throw cppLox::Error::RunTimeException(
        std::format("{}\n[line {}] in script\n", errorMessage, frame.m_function->chunk()->getLine(index)));
}