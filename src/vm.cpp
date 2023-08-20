#include "vm.hpp"

#include <iostream>

#include "object_formatter.hpp"
#include "object_string.hpp"
#include "opcode.hpp"
#include "runtime_exception.hpp"
#include "string_operations.hpp"


VM::VM(MemoryMutator * memoryMutator) {
    m_stack_top = 0;
    m_chunk = nullptr;
    m_instruction_index = 0;
    m_memoryMutator = memoryMutator;
}

VM::~VM() {
}

auto VM::interpret(Chunk & chunk) -> void {
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
        switch (static_cast<Opcode>(instruction)) {
        case Opcode::ADD:
            {
                Value a = pop();
                Value b = pop();
                if (a.is(Value::Type::NUMBER) && b.is(Value::Type::NUMBER)) {
                    push(a + b);
                } else if (a.is(Value::Type::OBJECT) && b.is(Value::Type::OBJECT)) {
                    Object * aObject = a.as<Object *>();
                    Object * bObject = b.as<Object *>();
                    if (aObject->is(Object::Type::STRING) && bObject->is(Object::Type::STRING)) {
                        push(Value(concatenate_strings(bObject->as<ObjectString>(), aObject->as<ObjectString>(),
                                                       m_memoryMutator)));
                    } else {
                        throw RunTimeException("Operands must be two numbers or two strings");
                    }
                }
                break;
            }
        case Opcode::CONSTANT:
            {
                uint8_t constant = chunk.getByte(m_instruction_index++);
                push(chunk.getConstant(constant));
                break;
            }
        case Opcode::DIVIDE:
            push(pop() / pop());
            break;
        case Opcode::EQUAL:
            push(pop() == pop());
            break;
        case Opcode::FALSE:
            push(Value(false));
            break;
        case Opcode::GREATER:
            push(pop() > pop());
            break;
        case Opcode::GREATER_EQUAL:
            push(pop() >= pop());
            break;
        case Opcode::LESS:
            push(pop() < pop());
            break;
        case Opcode::LESS_EQUAL:
            push(pop() <= pop());
            break;
        case Opcode::MULTIPLY:
            push(pop() * pop());
            break;
        case Opcode::NEGATE:
            push(-pop());
            break;
        case Opcode::NOT:
            push(!pop());
            break;
        case Opcode::NOT_EQUAL:
            push(pop() != pop());
            break;
        case Opcode::NULL_:
            push(Value());
            break;
        case Opcode::RETURN:
            return;
        case Opcode::PRINT:
            std::cout << pop() << std::endl;
            break;
        case Opcode::SUBTRACT:
            push(pop() - pop());
            break;
        case Opcode::TRUE:
            push(Value(true));
            break;
        }
    }

    return;
}

auto VM::push(Value value) -> void {
    if (m_stack_top == STACK_MAX) {
        runTimeError("Stack overflow");
    }
    m_stack[m_stack_top] = value;
    m_stack_top++;
}

[[nodiscard]] auto VM::pop() -> Value {
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
    throw RunTimeException(errorMessage);
}