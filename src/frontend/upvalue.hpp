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
 * @file upvalue.hpp
 * @brief This file contains the definition of the Upvalue class.
 */

#pragma once

#include <cstdint>

namespace cppLox::Frontend {
class Upvalue {
  public:
    Upvalue();

    /// @brief Constructs a new upvalue.
    /// @param index The index of the local variable or upvalue.
    /// @param isLocal Whether the upvalue is an upvalue or a local variable.
    Upvalue(uint8_t index, bool isLocal);

    /// @brief Destructs the upvalue.
    ~Upvalue() = default;

    /// @brief Gets the index of the local variable or upvalue.
    /// @return The index of the local variable or upvalue.
    [[nodiscard]] auto index() const noexcept -> uint8_t;

    /// @brief Gets whether the upvalue is an upvalue or a local variable.
    /// @return Whether the upvalue is an upvalue or a local variable.
    [[nodiscard]] auto isLocal() const noexcept -> bool;

  private:
    /// @brief The index of the local variable or upvalue.
    uint8_t m_index;

    /// @brief Whether the upvalue is an upvalue or a local variable.
    bool m_isLocal;
};
} // namespace cppLox::Frontend