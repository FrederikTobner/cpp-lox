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
 * @file native_functions.cpp
 * @brief This file contains the implementation of the native functions that are available to the cpplox interpreter
 */

#include "native_functions.hpp"

using namespace cppLox::Backend;

cppLox::Types::Value cppLox::Backend::clock(uint8_t argCount, cppLox::Types::Value * args, CallFrame & frame,
                                            std::function<void(CallFrame & frame, std::string_view fmt)> onError) {
    return cppLox::Types::Value(static_cast<double>(
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count()));
}