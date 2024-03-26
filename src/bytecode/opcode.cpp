/****************************************************************************
 * Copyright (C) 2024 by Frederik Tobner                                    *
 *                                                                          *
 * This file is part of cpp-lox.                                            *
 *                                                                          *
 * Permission to use, copy, modify, and distribute this software and its    *
 * documentation under the terms of the GNU General Public License is       *
 * hereby granted.                                                          *
 * No representations are made about the suitability of this software for   *
 * any purpose.                                                             *
 * It is provided "as is" without express or implied warranty.              *
 * See the <"https://www.gnu.org/licenses/gpl-3.0.html">GNU General Public  *
 * License for more details.                                                *
 ****************************************************************************/

/**
 * @file opcode.cpp
 * @brief This file contains the functions for the Opcode enum.
 */

#include "opcode.hpp"

[[nodiscard]] auto cppLox::ByteCode::opcode_as_string(const cppLox::ByteCode::Opcode value) -> std::string_view {
    switch (value) {
    case Opcode::ADD:
        return "ADD";
    case Opcode::CALL:
        return "CALL";
    case Opcode::CONSTANT:
        return "CONSTANT";
    case Opcode::DEFINE_GLOBAL:
        return "DEFINE_GLOBAL";
    case Opcode::DIVIDE:
        return "DIVIDE";
    case Opcode::EQUAL:
        return "EQUAL";
    case Opcode::FALSE:
        return "FALSE";
    case Opcode::GET_GLOBAL:
        return "GET_GLOBAL";
    case Opcode::GET_LOCAL:
        return "GET_LOCAL";
    case Opcode::GREATER:
        return "GREATER";
    case Opcode::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case Opcode::JUMP_IF_FALSE:
        return "JUMP_IF_FALSE";
    case Opcode::JUMP:
        return "JUMP";
    case Opcode::LESS:
        return "LESS";
    case Opcode::LESS_EQUAL:
        return "LESS_EQUAL";
    case Opcode::LOOP:
        return "LOOP";
    case Opcode::MULTIPLY:
        return "MULTIPLY";
    case Opcode::NEGATE:
        return "NEGATE";
    case Opcode::NOT:
        return "NOT";
    case Opcode::NOT_EQUAL:
        return "NOT_EQUAL";
    case Opcode::NULL_:
        return "NULL_";
    case Opcode::POP:
        return "POP";
    case Opcode::PRINT:
        return "PRINT";
    case Opcode::RETURN:
        return "RETURN";
    case Opcode::SET_GLOBAL:
        return "SET_GLOBAL";
    case Opcode::SET_LOCAL:
        return "SET_LOCAL";
    case Opcode::SUBTRACT:
        return "SUBTRACT";
    case Opcode::TRUE:
        return "TRUE";
    }
    return "Unknown opcode";
}

[[nodiscard]] auto cppLox::ByteCode::operator<<(std::ostream & os, Opcode const & opcode) -> std::ostream & {
    return os << cppLox::ByteCode::opcode_as_string(opcode);
}
