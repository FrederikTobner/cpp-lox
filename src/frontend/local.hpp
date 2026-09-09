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
 * @file local.hpp
 * @brief This file contains the declaration of the Local class.
 */

#pragma once

#include <cstdint>
#include <string>

#include "../language_features.hpp"
#include "token.hpp"


namespace cppLox::Frontend {

// Forward declaration to avoid circular dependency
class LocalScope;

/// @brief Models a local variable.
class Local {

    friend class LocalScope;

  public:
    /// @brief Constructor of the local variable.
    /// @param token The token of the name of the local variable.
    /// @param depth The depth of the local variable.
    Local(cppLox::Frontend::Token & token, int32_t depth);

    /// @brief Gets the token of the name of the local variable.
    auto getToken() const _NO_EXCEPT->cppLox::Frontend::Token const &;

    /// @brief Gets the depth of the local variable.
    auto getDepth() const _NO_EXCEPT->int32_t;

    /// @brief Gets whether the local variable is captured by a closure as an upvalue.
    /// @return Whether the local variable is captured.
    auto isCaptured() const _NO_EXCEPT->bool;

    /// @brief Marks the local variable as captured by a closure as an upvalue.
    auto markCaptured() _NO_EXCEPT->void;

  private:
    /// @brief The token of the name of the local variable.
    cppLox::Frontend::Token m_token;

    /// @brief The depth of the local variable.
    int32_t m_depth;

    /// @brief Whether the local variable is captured by a closure as an upvalue.
    bool m_isCaptured = false;
};
} // namespace cppLox::Frontend