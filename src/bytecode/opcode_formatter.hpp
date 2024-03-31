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
 * @file opcode_formatter.hpp
 * @brief This file contains the formatter for the Opcode enum.
 */

#pragma once

#include <format>
#include <string>

#include "opcode.hpp"

/// @brief Formatter for the Opcode enum
template <> struct std::formatter<cppLox::ByteCode::Opcode> : std::formatter<std::string_view> {

    /// @brief Formats the opcode as a string.
    /// @param opcode The opcode to format.
    /// @param ctx The format context.
    /// @return The formatted string.
    [[nodiscard]] inline auto format(cppLox::ByteCode::Opcode opcode, format_context & ctx) const {
        return formatter<string_view>::format(std::format("{}", opcode_as_string(opcode)), ctx);
    }
};