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

ObjectUpValue::ObjectUpValue(Value * location) : m_location(location) {
    m_type = Object::Type::UPVALUE;
}

auto ObjectUpValue::location() const _NO_EXCEPT->Value * {
    return m_location;
}

auto ObjectUpValue::setLocation(Value * location) -> void {
    m_location = location;
}

auto ObjectUpValue::isClosed() const _NO_EXCEPT->bool {
    return m_location == &m_closedValue;
}

auto ObjectUpValue::close() -> void {
    m_closedValue = *m_location;
    m_location = &m_closedValue;
}