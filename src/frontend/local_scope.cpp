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
 * @file local_scope.cpp
 * @brief This file contains the implementation of the LocalScope class.
 */

#include "local_scope.hpp"

using namespace cppLox::Frontend;

LocalScope::LocalScope() : m_localCount(0) {
}

LocalScope::LocalScope(std::shared_ptr<LocalScope> enclosing) : m_enclosing(enclosing), m_localCount(0) {
}

auto LocalScope::enclosing() const -> std::optional<std::shared_ptr<LocalScope>> const & {
    return m_enclosing;
}

auto LocalScope::local(uint16_t index) const -> Local const & {
    return *m_locals[index];
}

auto LocalScope::localCount() const -> uint16_t const & {
    return m_localCount;
}

auto LocalScope::addLocal(cppLox::Frontend::Token name, std::function<void(std::string &)> onError) -> void {
    if (m_localCount == 256) {
        std::string message = "Too many local variables in function";
        onError(message);
    }
    m_locals[m_localCount] = std::make_unique<Local>(name, -1);
    m_localCount++;
}

auto LocalScope::markInitialized(uint16_t depth) -> void {
    m_locals[m_localCount - 1]->m_depth = depth;
}

auto LocalScope::popLocal() -> void {
    m_localCount--;
}