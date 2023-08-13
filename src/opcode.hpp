#pragma once

/// @brief The opcodes of the intermidiate language
typedef enum {
    OP_ADD,
    OP_CONSTANT,
    OP_DIVIDE,
    OP_EQUAL,
    OP_FALSE,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_MULTIPLY,
    OP_NEGATE,
    OP_NOT,
    OP_NOT_EQUAL,
    OP_NULL,
    OP_PRINT,
    OP_RETURN,
    OP_SUBTRACT,
    OP_TRUE,
} Opcode;