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
 * @file object_formatter.hpp
 * @brief This file contains the formatter for the Object class.
 */

#pragma once

#include <format>

#include "object.hpp"
#include "object_function.hpp"
#include "object_string.hpp"

/// @brief Formatter for the Object class.
template <> struct std::formatter<cppLox::Types::Object *> : std::formatter<std::string_view> {

    /// @brief Formats the object as a string.
    /// @param object The object to format.
    /// @param ctx The format context.
    /// @return The formatted string.
    [[nodiscard]] auto format(cppLox::Types::Object * object, format_context & ctx) const {
        switch (object->type()) {
        case cppLox::Types::Object::Type::STRING:
            return formatter<string_view>::format(
                std::format("{}", object->as<cppLox::Types::ObjectString>()->string()), ctx);
        case cppLox::Types::Object::Type::FUNCTION:
            {
                cppLox::Types::ObjectString * name = object->as<cppLox::Types::ObjectFunction>()->name();
                if (name != nullptr) {
                    return formatter<string_view>::format(std::format("<fn {}>", name->string()), ctx);
                }
                return formatter<string_view>::format(std::format("<script>"), ctx);
            }
        case cppLox::Types::Object::Type::NATIVE_FUNCTION:
            {
                return formatter<string_view>::format(std::format("<native fn>"), ctx);
            }
        }

        // should be be unreachable
        return formatter<string_view>::format(std::format("undefined"), ctx);
    }
};