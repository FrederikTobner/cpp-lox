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
 * @file object_upvalue.cpp
 * @brief This file contains the implementation of the ObjectUpValue class.
 */

#include "object_upvalue.hpp"

using namespace cppLox::Types;

ObjectUpValue::ObjectUpValue(Value * closed) : m_closed(closed) {
    m_type = Object::Type::UPVALUE;
}

auto ObjectUpValue::closed() const noexcept -> Value * {
    return m_closed;
}

auto ObjectUpValue::setClosed(Value * closed) -> void {
    m_closed = closed;
}