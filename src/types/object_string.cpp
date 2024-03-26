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
 * @file object_string.cpp
 * @brief This file contains the implementation of the ObjectString class.
 */

#include "object_string.hpp"

using namespace cppLox::Types;

ObjectString::ObjectString(std::string const & string) {
    m_string = std::make_unique<std::string>(string);
    m_type = Object::Type::STRING;
}

[[nodiscard]] auto ObjectString::string() const -> std::string const & {
    return *m_string.get();
}

auto ObjectString::operator==(ObjectString const & other) const -> bool {
    return *m_string.get() == other.string();
}

auto ObjectString::operator==(ObjectString const * other) const -> bool {
    return *m_string.get() == other->string();
}