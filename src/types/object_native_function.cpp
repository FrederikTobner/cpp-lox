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

#include "object_native_fuction.hpp"

using namespace cppLox::Types;

auto ObjectNativeFunction::call(uint8_t argCount, Value * args, cppLox::Backend::CallFrame & frame,
                                std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)
    -> Value {
    return m_function(argCount, args, frame, onError);
}

auto ObjectNativeFunction::arity() const -> int16_t {
    return m_arity;
}