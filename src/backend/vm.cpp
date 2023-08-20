#include "vm.hpp"

#include <iostream>

#include "../bytecode/opcode.hpp"
#include "../error/runtime_exception.hpp"
#include "../types/object_formatter.hpp"
#include "../types/object_string.hpp"

using namespace cppLox::Backend;

VM::VM(MemoryMutator * memoryMutator) {
    m_stack_top = 0;
    m_chunk = nullptr;
    m_instruction_index = 0;
    m_memoryMutator = memoryMutator;
}

VM::~VM() {
}

auto VM::interpret(cppLox::ByteCode::Chunk & chunk) -> void {
    this->m_instruction_index = 0;
    this->m_chunk = &chunk;
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (Value * slot = m_stack; slot < m_stack_top; slot++) {
            std::cout << std::format("[ {} ]", *slot);
        }
        std::cout << std::endl;
        chunk_disassemble_instruction(chunk, m_instruction_index);
#endif

        uint8_t instruction = chunk.getByte(m_instruction_index++);
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
                uint8_t constant = chunk.getByte(m_instruction_index++);
                push(chunk.getConstant(constant));
                break;
            }
        case cppLox::ByteCode::Opcode::DIVIDE:
            push(pop() / pop());
            break;
        case cppLox::ByteCode::Opcode::EQUAL:
            push(pop() == pop());
            break;
        case cppLox::ByteCode::Opcode::FALSE:
            push(cppLox::Types::Value(false));
            break;
        case cppLox::ByteCode::Opcode::GREATER:
            push(pop() > pop());
            break;
        case cppLox::ByteCode::Opcode::GREATER_EQUAL:
            push(pop() >= pop());
            break;
        case cppLox::ByteCode::Opcode::LESS:
            push(pop() < pop());
            break;
        case cppLox::ByteCode::Opcode::LESS_EQUAL:
            push(pop() <= pop());
            break;
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
        case cppLox::ByteCode::Opcode::SUBTRACT:
            push(pop() - pop());
            break;
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

auto VM::resetStack() -> void {
    m_stack_top = 0;
}

template <class... Args> auto VM::runTimeError(std::string_view fmt, Args &&... args) -> void {
    std::string errorMessage = std::vformat(fmt, std::make_format_args(args...));
    errorMessage += "\n";
    size_t instruction = m_instruction_index - 1;
    errorMessage.append(std::format("[line {}] in script\n", m_chunk->getLine(instruction)));
    throw cppLox::Error::RunTimeException(errorMessage);
}