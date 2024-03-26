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
 * @file function_type.hpp
 * @brief This file contains the FunctionType enum.
 */

#pragma once

#include <cstdint>

namespace cppLox::Frontend {
/// @brief The type of a function.
enum class FunctionType : uint8_t {
    /// @brief The type of a function
    FUNCTION,
    /// @brief The type of top-level code
    SCRIPT,
};

} // namespace cppLox::Frontend
