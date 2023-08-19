#include "vm.hpp"

#include <iostream>

#include "opcode.hpp"
#include "runtime_exception.hpp"

VM::VM(MemoryManager * memoryManager) {
    m_stack_top = 0;
    m_chunk = nullptr;
    m_instruction_index = 0;
    m_memoryManager = memoryManager;
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
            {
                Value a = pop();
                Value b = pop();
                if (a.is(Value::Type::VAL_NUMBER) && b.is(Value::Type::VAL_NUMBER)) {
                    push(a + b);
                } else if (a.is(Value::Type::VAL_OBJECT) && b.is(Value::Type::VAL_OBJECT)) {
                    Object * aObject = a.asObject();
                    Object * bObject = b.asObject();
                    if (aObject->is(Object::Type::OBJ_STRING) && bObject->is(Object::Type::OBJ_STRING)) {
                        push(Value(
                            m_memoryManager->concatenate(bObject->as<ObjectString>(), aObject->as<ObjectString>())));
                    } else {
                        throw RunTimeException("Operands must be two numbers or two strings");
                    }
                }
                break;
            }
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
    if (m_stack_top == STACK_MAX) {
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

void VM::resetStack() {
    m_stack_top = 0;
}

template <class... Args> void VM::runTimeError(std::string_view fmt, Args &&... args) {
    std::string errorMessage = std::vformat(fmt, std::make_format_args(args...));
    errorMessage += "\n";
    size_t instruction = m_instruction_index - 1;
    errorMessage.append(std::format("[line {}] in script\n", m_chunk->getLine(instruction)));
    throw RunTimeException(errorMessage);
}