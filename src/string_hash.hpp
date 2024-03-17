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
 * @file string_hash.hpp
 * @brief This file contains the hash specialization for ObjectString.
 */

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "types/object_string.hpp"

namespace std {

/// @brief Hashes the given ObjectString reference or pointer.
template <> struct hash<cppLox::Types::ObjectString> {

    /// @brief Hashes the given ObjectString reference.
    /// @param s The ObjectString to hash.
    /// @return The hash of the given string.
    [[nodiscard]] auto operator()(cppLox::Types::ObjectString & s) const -> std::size_t {
        return std::hash<std::string_view>{}(s.string());
    }

    /// @brief Hashes the given ObjectString pointer.
    /// @param s The ObjectString pointer to hash.
    /// @return The hash of the given string.
    [[nodiscard]] auto operator()(cppLox::Types::ObjectString * s) const -> std::size_t {
        return std::hash<std::string_view>{}(s->string());
    }
};
} // namespace std