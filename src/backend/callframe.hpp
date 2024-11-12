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
 * @file callframe.hpp
 * @details This file contains the declaration of the call frame used by the virtual machine
 */

#pragma once

#include <cstdint>

#include "../language_features.hpp"
#include "../types/object_closure.hpp"
#include "../types/value.hpp"


namespace cppLox::Backend {

// forward declaration of the VM class
class VM;

class CallFrame {

    // The VM class is a friend of the CallFrame class
    friend class VM;

  public:
    /// @brief Constructs a new call frame.
    CallFrame() = default;

    /// @brief Constructs a new call frame.
    /// @param closure The closure to use.
    /// @param slots The slots to use.
    CallFrame(cppLox::Types::ObjectClosure * closure, cppLox::Types::Value * slots);

    /// @brief Gets the closure of the call frame.
    /// @return The closure of the call frame.
    [[nodiscard]] auto closure() const _NO_EXCEPT->cppLox::Types::ObjectClosure *;

    /// @brief Gets the instruction pointer of the call frame.
    /// @return The instruction pointer of the call frame.
    [[nodiscard]] auto instructionPointer() const _NO_EXCEPT->uint8_t *;

    /// @brief Gets the slots of the call frame.
    /// @return The slots of the call frame.
    [[nodiscard]] auto slots() const _NO_EXCEPT->cppLox::Types::Value *;

  private:
    /// @brief The function of the call frame.
    cppLox::Types::ObjectClosure * m_closure;

    /// @brief The instruction pointer of the call frame.
    uint8_t * m_instruction_pointer;

    /// @brief The slots of the call frame.
    cppLox::Types::Value * m_slots;
};
} // namespace cppLox::Backend