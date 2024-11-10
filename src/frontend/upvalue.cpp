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
 * @file upvalue.cpp
 * @brief This file contains the implementation of the Upvalue class.
 */

#include "upvalue.hpp"

using namespace cppLox::Frontend;

Upvalue::Upvalue() : m_index(0), m_isLocal(false) {
}

Upvalue::Upvalue(uint8_t index, bool isLocal) : m_index(index), m_isLocal(isLocal) {
}

auto Upvalue::index() const noexcept -> uint8_t {
    return m_index;
}

auto Upvalue::isLocal() const noexcept -> bool {
    return m_isLocal;
}