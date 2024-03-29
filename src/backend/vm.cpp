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
    m_chunk = nullptr;
    m_instruction_index = 0;
    m_memoryMutator = memoryMutator;
}

auto VM::interpret(cppLox::ByteCode::Chunk & chunk) -> void {
    this->m_instruction_index = 0;
    this->m_chunk = &chunk;
#ifdef DEBUG_PRINT_BYTECODE
    chunk.disassemble("script");
#endif
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (cppLox::Types::Value * slot = m_stack; (size_t)slot < m_stack_top; slot++) {
            std::cout << std::format("[ {} ]", *slot);
        }
        std::cout << std::endl;
        chunk.disassembleInstruction(m_instruction_index);
#endif
        uint8_t const instruction = chunk.getByte(m_instruction_index++);
        switch (static_cast<cppLox::ByteCode::Opcode>(instruction)) {
        case cppLox::ByteCode::Opcode::ADD:
            {
                cppLox::Types::Value a = pop();
                cppLox::Types::Value b = pop();
                if (a.is(cppLox::Types::Value::Type::NUMBER) && b.is(cppLox::Types::Value::Type::NUMBER)) {
                    push(a + b);
                } else if (a.is(cppLox::Types::Value::Type::OBJECT) && b.is(cppLox::Types::Value::Type::OBJECT)) {
                    cppLox::Types::Object * aObject = a.as<cppLox::Types::Object *>();
                    cppLox::Types::Object * bObject = b.as<cppLox::Types::Object *>();
                    if (aObject->is(cppLox::Types::Object::Type::STRING) &&
                        bObject->is(cppLox::Types::Object::Type::STRING)) {
                        push(cppLox::Types::Value(m_memoryMutator->create<cppLox::Types::ObjectString>(
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
                uint8_t const constant = chunk.getByte(m_instruction_index++);
                push(chunk.getConstant(constant));
                break;
            }
        case cppLox::ByteCode::Opcode::DEFINE_GLOBAL:
            {
                uint8_t const constant = chunk.getByte(m_instruction_index++);
                cppLox::Types::Value const value = pop();
                if (m_memoryMutator->setGlobal(
                        chunk.getConstant(constant).as<cppLox::Types::Object *>()->as<cppLox::Types::ObjectString>(),
                        value)) {
                    throw cppLox::Error::RunTimeException("Variable already defined");
                }
                push(value);
                break;
            }
        case cppLox::ByteCode::Opcode::DIVIDE:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b / a);
                break;
            }
        case cppLox::ByteCode::Opcode::EQUAL:
            push(pop() == pop());
            break;
        case cppLox::ByteCode::Opcode::FALSE:
            push(cppLox::Types::Value(false));
            break;
        case cppLox::ByteCode::Opcode::GET_GLOBAL:
            {
                uint8_t const constant = chunk.getByte(m_instruction_index++);
                cppLox::Types::Value const value = m_memoryMutator->getGlobal(
                    chunk.getConstant(constant).as<cppLox::Types::Object *>()->as<cppLox::Types::ObjectString>());
                push(value);
                break;
            }
        case cppLox::ByteCode::Opcode::GET_LOCAL:
            {
                uint8_t const slot = chunk.getByte(m_instruction_index++);
                push(m_stack[slot]);
                break;
            }
        case cppLox::ByteCode::Opcode::GREATER:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b > a);
                break;
            }
        case cppLox::ByteCode::Opcode::GREATER_EQUAL:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b >= a);
                break;
            }
        case cppLox::ByteCode::Opcode::JUMP:
            {
                uint16_t const offset = getShort(chunk, m_instruction_index);
                m_instruction_index += offset;
                break;
            }
        case cppLox::ByteCode::Opcode::JUMP_IF_FALSE:
            {
                uint16_t const offset = getShort(chunk, m_instruction_index);
                if ((!peek()).as<bool>()) {
                    m_instruction_index += offset;
                }
                break;
            }
        case cppLox::ByteCode::Opcode::LESS:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b < a);
                break;
            }
        case cppLox::ByteCode::Opcode::LESS_EQUAL:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b <= a);
                break;
            }
        case cppLox::ByteCode::Opcode::LOOP:
            {
                uint16_t const offset = getShort(chunk, m_instruction_index);
                m_instruction_index -= offset;
                break;
            }
        case cppLox::ByteCode::Opcode::MULTIPLY:
            push(pop() * pop());
            break;
        case cppLox::ByteCode::Opcode::NEGATE:
            push(-pop());
            break;
        case cppLox::ByteCode::Opcode::NOT:
            push(!pop());
            break;
        case cppLox::ByteCode::Opcode::NOT_EQUAL:
            push(pop() != pop());
            break;
        case cppLox::ByteCode::Opcode::NULL_:
            push(cppLox::Types::Value());
            break;
        case cppLox::ByteCode::Opcode::RETURN:
            return;
        case cppLox::ByteCode::Opcode::PRINT:
            std::cout << pop() << std::endl;
            break;
        case cppLox::ByteCode::Opcode::SET_GLOBAL:
            {
                uint8_t const constant = chunk.getByte(m_instruction_index++);
                cppLox::Types::Value value = pop();
                cppLox::Types::ObjectString * name =
                    chunk.getConstant(constant).as<cppLox::Types::Object *>()->as<cppLox::Types::ObjectString>();
                if (!m_memoryMutator->setGlobal(name, value)) {
                    m_memoryMutator->deleteGlobal(name);
                    runTimeError("Undefined variable '%s'", name->string().c_str());
                };
                break;
            }
        case cppLox::ByteCode::Opcode::SET_LOCAL:
            {
                uint8_t const slot = chunk.getByte(m_instruction_index++);
                m_stack[slot] = peek();
                break;
            }
        case cppLox::ByteCode::Opcode::SUBTRACT:
            {
                cppLox::Types::Value const a = pop();
                cppLox::Types::Value const b = pop();
                push(b - a);
                break;
            }
        case cppLox::ByteCode::Opcode::TRUE:
            push(cppLox::Types::Value(true));
            break;
        }
    }

    return;
}

auto VM::push(cppLox::Types::Value value) -> void {
    if (m_stack_top == STACK_MAX) {
        runTimeError("Stack overflow");
    }
    m_stack[m_stack_top] = value;
    m_stack_top++;
}

[[nodiscard]] auto VM::pop() -> cppLox::Types::Value {
    if (m_stack_top == 0) {
        runTimeError("Stack empty on pop");
    }
    m_stack_top--;
    return m_stack[m_stack_top];
}

[[nodiscard]] auto VM::peek() -> cppLox::Types::Value {
    if (m_stack_top == 0) {
        runTimeError("Stack empty on peek");
    }
    return m_stack[m_stack_top - 1];
}

auto VM::resetStack() -> void {
    m_stack_top = 0;
}

[[nodiscard]] auto VM::getShort(cppLox::ByteCode::Chunk & chunk, size_t offset) -> uint16_t {
    m_instruction_index += 2;
    return static_cast<uint16_t>(chunk.getByte(offset) << 8 | chunk.getByte(offset + 1));
}

template <class... Args> auto VM::runTimeError(std::string_view fmt, Args &&... args) -> void {
    std::string const errorMessage = std::vformat(fmt, std::make_format_args(args...));
    size_t const instructionIndex = m_instruction_index - 1;
    throw cppLox::Error::RunTimeException(
        std::format("{}\n[line {}] in script\n", errorMessage, m_chunk->getLine(instructionIndex)));
}