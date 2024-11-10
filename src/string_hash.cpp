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
 * @file string_hash.cpp
 * @brief This file contains the implementation of the hash specialization for ObjectString.
 */

#include "string_hash.hpp"

namespace std {
auto hash<cppLox::Types::ObjectString>::operator()(cppLox::Types::ObjectString & s) const -> std::size_t {
    return std::hash<std::string_view>{}(s.string());
}

auto hash<cppLox::Types::ObjectString>::operator()(cppLox::Types::ObjectString * s) const -> std::size_t {
    return std::hash<std::string_view>{}(s->string());
}
} // namespace std