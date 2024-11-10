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
 * @file object_closure.cpp
 * @brief This file contains the implementation of the ObjectClosure class.
 */

#include "object_closure.hpp"

#include <ranges>

using namespace cppLox::Types;

ObjectClosure::ObjectClosure(ObjectFunction * function) : m_function(function) {
    m_type = Object::Type::CLOSURE;
    for (auto i : std::views::iota(0u, function->upvalueCount())) {
        m_upvalues.push_back(nullptr);
    }
}

auto ObjectClosure::function() const noexcept -> ObjectFunction * {
    return m_function;
}

auto ObjectClosure::upvalues() noexcept -> std::vector<ObjectUpValue *> & {
    return m_upvalues;
}