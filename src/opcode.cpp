#include "opcode.hpp"

#include "assert.hpp"
#include <map>

std::string const & opcode_as_string(Opcode value) {
    static std::map<Opcode, std::string> opcodeNames;
    if (opcodeNames.size() == 0) {
#define INSERT_ELEMENT(p) opcodeNames[p] = #p
        INSERT_ELEMENT(OP_ADD);
        INSERT_ELEMENT(OP_CONSTANT);
        INSERT_ELEMENT(OP_DIVIDE);
        INSERT_ELEMENT(OP_EQUAL);
        INSERT_ELEMENT(OP_FALSE);
        INSERT_ELEMENT(OP_GREATER);
        INSERT_ELEMENT(OP_GREATER_EQUAL);
        INSERT_ELEMENT(OP_LESS);
        INSERT_ELEMENT(OP_LESS_EQUAL);
        INSERT_ELEMENT(OP_MULTIPLY);
        INSERT_ELEMENT(OP_NEGATE);
        INSERT_ELEMENT(OP_NOT);
        INSERT_ELEMENT(OP_NOT_EQUAL);
        INSERT_ELEMENT(OP_NULL);
        INSERT_ELEMENT(OP_PRINT);
        INSERT_ELEMENT(OP_RETURN);
        INSERT_ELEMENT(OP_SUBTRACT);
        INSERT_ELEMENT(OP_TRUE);
#undef INSERT_ELEMENT
        ASSERT(opcodeNames.size() == COUNT, "Not all opcodes have been added to the map");
    }
    return opcodeNames[value];
}

std::ostream & operator<<(std::ostream & out, Opcode value) {
    return out << opcode_as_string(value);
}