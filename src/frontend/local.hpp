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

#include <string>

#include <cstddef>

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
    Local(cppLox::Frontend::Token & token, std::int32_t depth);

    /// @brief Gets the token of the name of the local variable.
    auto getToken() const noexcept -> cppLox::Frontend::Token const &;

    /// @brief Gets the depth of the local variable.
    auto getDepth() const noexcept -> std::int32_t;

  private:
    /// @brief The token of the name of the local variable.
    cppLox::Frontend::Token m_token;

    /// @brief The depth of the local variable.
    std::int32_t m_depth;
};
} // namespace cppLox::Frontend