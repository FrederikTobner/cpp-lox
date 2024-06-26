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
 * @file token_formatter.hpp
 * @brief This file contains the formatter for the Token class.
 */

#pragma once

#include <format>
#include <string>

#include "token.hpp"

/// @brief Formatter for the Token class
template <> struct std::formatter<cppLox::Frontend::Token> : std::formatter<std::string_view> {

    /// @brief Formats the given token
    /// @param token The token to format
    /// @param ctx The format context
    /// @return The formatted token
    [[nodiscard]] auto format(cppLox::Frontend::Token token, format_context & ctx) const {
        return formatter<string_view>::format(std::format("Token({}, {})", token.lexeme(), token.line()), ctx);
    }
};