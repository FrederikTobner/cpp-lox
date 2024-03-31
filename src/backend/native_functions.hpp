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
 * @file native_functions.hpp
 * @brief This file contains the declaration of the native functions that are available to the cpplox interpreter
 */

#pragma once

#include <chrono>
#include <functional>

#include "../types/value.hpp"
#include "callframe.hpp"

namespace cppLox::Backend {

/// @brief The clock function.
/// @param argCount The number of arguments that were passed to the function
/// @param args The arguments that were passed to the function
/// @param frame The call frame from which the function was called
/// @param onError The error function to call if an error occurs
/// @return The amount of time in milliseconds since the Unix epoch.
cppLox::Types::Value clock(uint8_t argCount, Types::Value * args, CallFrame & frame,
                           std::function<void(CallFrame & frame, std::string_view fmt)> onError);
} // namespace cppLox::Backend