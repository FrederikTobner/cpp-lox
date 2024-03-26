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
 * @file callframe.cpp
 * @brief This file contains the implementation of the CallFrame class.
 */

#include "callframe.hpp"

using namespace cppLox::Backend;

CallFrame::CallFrame(cppLox::Types::ObjectFunction * function, cppLox::Types::Value * slots)
    : m_function(function), m_slots(slots) {
    m_instruction_pointer = function->chunk()->code().data();
}

[[nodiscard]] auto CallFrame::function() const noexcept -> cppLox::Types::ObjectFunction * {
    return m_function;
}

[[nodiscard]] auto CallFrame::instructionPointer() const noexcept -> uint8_t * {
    return m_instruction_pointer;
}

[[nodiscard]] auto CallFrame::slots() const noexcept -> cppLox::Types::Value * {
    return m_slots;
}