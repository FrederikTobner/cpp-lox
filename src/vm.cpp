#include "vm.hpp"

#include <iostream>

#include "opcode.hpp"

VM::VM() {
    this->m_instruction_index = 0;
    this->m_stack_top = this->m_stack;
}

VM::~VM() {
}

[[nodiscard]] VM::InterpretResult VM::interpret(Chunk & chunk) {
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        for (Value * slot = this->m_stack; slot < this->m_stack_top; slot++) {
            std::cout << "[ ";
            std::cout << *slot << " ";
            std::cout << "]";
        }
        std::cout << std::endl;
        chunk_disassemble_instruction(chunk, this->m_instruction_index);
#endif

        uint8_t instruction = chunk.getByte(this->m_instruction_index++);
        switch (instruction) {
        case OP_ADD:
            this->push(this->pop() + this->pop());
            break;
        case OP_CONSTANT:
            {
                uint8_t constant = chunk.getByte(this->m_instruction_index++);
                this->push(chunk.getConstant(constant));
                break;
            }
        case OP_DIVIDE:
            this->push(this->pop() / this->pop());
            break;
        case OP_MULTIPLY:
            this->push(this->pop() * this->pop());
            break;
        case OP_NEGATE:
            this->push(-this->pop());
            break;
        case OP_RETURN:
            return VM::InterpretResult::INTERPRET_OK;
        case OP_SUBTRACT:
            this->push(this->pop() - this->pop());
            break;
        }
    }

    return VM::InterpretResult::INTERPRET_OK;
}

void VM::push(Value value) {
    if (this->m_stack_top - this->m_stack == STACK_MAX) {
        std::cout << "Stack overflow" << std::endl;
        exit(1);
    }
    *this->m_stack_top = value;
    this->m_stack_top++;
}

[[nodiscard]] Value VM::pop() {
    if (this->m_stack_top == this->m_stack) {
        std::cout << "Stack underflow" << std::endl;
        exit(1);
    }
    this->m_stack_top--;
    return *this->m_stack_top;
}