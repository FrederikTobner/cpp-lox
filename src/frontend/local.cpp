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
 * @file local.cpp
 * @brief This file contains the implementation of the Local class.
 */

#include "local.hpp"

cppLox::Frontend::Local::Local(cppLox::Frontend::Token & token, std::int32_t depth) : m_depth(depth), m_token(token) {
}

auto cppLox::Frontend::Local::getToken() const noexcept -> cppLox::Frontend::Token const & {
    return m_token;
}

auto cppLox::Frontend::Local::getDepth() const noexcept -> int32_t {
    return m_depth;
}