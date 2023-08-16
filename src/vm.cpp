#include "vm.hpp"

#include <iostream>

#include "opcode.hpp"
#include "runtime_exception.hpp"

VM::VM() {
    m_stack_top = 0;
    m_chunk = nullptr;
    m_instruction_index = 0;
}

VM::~VM() {
}

void VM::interpret(Chunk & chunk) {
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
        switch (instruction) {
        case OP_ADD:
            push(pop() + pop());
            break;
        case OP_CONSTANT:
            {
                uint8_t constant = chunk.getByte(m_instruction_index++);
                push(chunk.getConstant(constant));
                break;
            }
        case OP_DIVIDE:
            push(pop() / pop());
            break;
        case OP_EQUAL:
            push(pop() == pop());
            break;
        case OP_FALSE:
            push(Value(false));
            break;
        case OP_GREATER:
            push(pop() > pop());
            break;
        case OP_GREATER_EQUAL:
            push(pop() >= pop());
            break;
        case OP_LESS:
            push(pop() < pop());
            break;
        case OP_LESS_EQUAL:
            push(pop() <= pop());
            break;
        case OP_MULTIPLY:
            push(pop() * pop());
            break;
        case OP_NEGATE:
            push(-pop());
            break;
        case OP_NOT:
            push(!pop());
            break;
        case OP_NOT_EQUAL:
            push(pop() != pop());
            break;
        case OP_NULL:
            push(Value());
            break;
        case OP_RETURN:
            return;
        case OP_PRINT:
            std::cout << pop() << std::endl;
            break;
        case OP_SUBTRACT:
            push(pop() - pop());
            break;
        case OP_TRUE:
            push(Value(true));
            break;
        }
    }

    return;
}

void VM::push(Value value) {
    if (m_stack_top == (size_t)STACK_MAX) {
        runTimeError("Stack overflow");
    }
    m_stack[m_stack_top] = value;
    m_stack_top++;
}

[[nodiscard]] Value VM::pop() {
    if (m_stack_top == 0) {
        runTimeError("Stack overflow");
    }
    m_stack_top--;
    return m_stack[m_stack_top];
}

template <class... Args> void VM::runTimeError(std::string_view fmt, Args &&... args) {
    std::string errorMessage = std::vformat(fmt, std::make_format_args(args...));
    errorMessage += "\n";
    size_t instruction = m_instruction_index - 1;
    errorMessage.append(std::format("[line {}] in script\n", m_chunk->getLine(instruction)));
    throw RunTimeException(errorMessage);
}