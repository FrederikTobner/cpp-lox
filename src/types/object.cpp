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
 * @file object.cpp
 * @brief This file contains the implementation of the Object class.
 */

#include "object.hpp"

using namespace cppLox::Types;

/// @brief Gets the type of the object.
/// @return The type of the object.
[[nodiscard]] auto Object::type() const -> Object::Type {
    return m_type;
}

/// @brief Checks if the object is of the given type.
/// @param type The type to check against.
/// @return True if the object is of the given type, false otherwise.
[[nodiscard]] auto Object::is(Type type) const -> bool {
    return m_type == type;
}

/// @brief Prints the value to the given output stream
/// @param os The output stream to print to
/// @param dt The value to print
/// @return The output stream
auto cppLox::Types::operator<<(std::ostream & os, Object const & dt) -> std::ostream & {
    dt.writeToOutputStream(os);
    return os;
}

/// @brief Prints the value to the given output stream
/// @param os The output stream to print to
/// @param dt The value to print
/// @return The output stream
auto cppLox::Types::operator<<(std::ostream & os, Object * dt) -> std::ostream & {
    dt->writeToOutputStream(os);
    return os;
}