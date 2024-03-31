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
 * @file simple_comperator.hpp
 * @brief This file contains a simple comperator for comparing two references or two pointers.
 */

#pragma once

#include "types/object_string.hpp"

namespace cppLox::Types {

template <typename T>
concept HasEqualOperator = requires(T & a, T & b) { a == b; };

/// @brief Simple comperator for comparing two references or two pointers.
/// @tparam T The type of the references / pointers.
template <HasEqualOperator T> struct SimpleComperator {
    /// @brief Compares two references.
    /// @param a The first reference.
    /// @param b The second reference.
    /// @return True if the references are equal, false otherwise.
    [[nodiscard]] auto operator()(T const & a, T const & b) const -> bool {
        return a == b;
    }

    /// @brief Compares two pointers.
    /// @param a The first pointer.
    /// @param b The second pointer.
    /// @return True if the pointers are equal, false otherwise.
    [[nodiscard]] auto operator()(T const * a, T const * b) const -> bool {
        return (*a) == b;
    }
};
} // namespace cppLox::Types
