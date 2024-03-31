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
 * @file value_formatter.hpp
 * @brief This file contains the formatter for the Value class.
 */

#pragma once

#include <format>
#include <string>

#include "object_formatter.hpp"
#include "value.hpp"

/// @brief Formatter for the Value class
template <> struct std::formatter<cppLox::Types::Value> : std::formatter<std::string_view> {
    [[nodiscard]] auto format(cppLox::Types::Value value, format_context & ctx) const {
        switch (value.getType()) {
        case cppLox::Types::Value::Type::BOOL:
            return formatter<string_view>::format(std::format("{}", value.as<bool>()), ctx);
        case cppLox::Types::Value::Type::NULL_:
            return formatter<string_view>::format(std::format("null"), ctx);
        case cppLox::Types::Value::Type::NUMBER:
            return formatter<string_view>::format(std::format("{}", value.as<double>()), ctx);
        case cppLox::Types::Value::Type::OBJECT:
            return formatter<string_view>::format(std::format("{}", value.as<cppLox::Types::Object *>()), ctx);
        }
        // should be be unreachable
        return formatter<string_view>::format(std::format("undefined"), ctx);
    }
};