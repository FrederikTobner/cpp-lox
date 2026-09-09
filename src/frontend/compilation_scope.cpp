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
 * @file compilation_scope.cpp
 * @brief This file contains the implementation of the CompilationScope class.
 */

#include "compilation_scope.hpp"

#include <ranges>

using namespace cppLox::Frontend;

CompilationScope::CompilationScope(cppLox::Types::ObjectFunction * function, FunctionType type) {
    m_enclosing = nullptr;
    m_function = function;
    m_scopeDepth = 0;
    m_currentFunctionType = type;
    m_localScope = std::make_shared<LocalScope>();
}
CompilationScope::CompilationScope(std::shared_ptr<CompilationScope> enclosing,
                                   cppLox::Types::ObjectFunction * function, FunctionType type) {
    m_enclosing = enclosing;
    m_function = function;
    m_scopeDepth = 0;
    m_currentFunctionType = type;
    m_localScope = std::make_shared<LocalScope>();
}

auto CompilationScope::enclosing() const -> std::shared_ptr<CompilationScope> const & {
    return m_enclosing;
}

auto CompilationScope::function() const -> cppLox::Types::ObjectFunction * {
    return m_function;
}

auto CompilationScope::currentFunctionType() const -> FunctionType const & {
    return m_currentFunctionType;
}

auto CompilationScope::scopeDepth() const -> uint16_t const & {
    return m_scopeDepth;
}

auto CompilationScope::localScope() const -> std::shared_ptr<LocalScope> const & {
    return m_localScope;
}

auto CompilationScope::beginNewScope() -> void {
    m_scopeDepth++;
    m_localScope = std::make_shared<LocalScope>(m_localScope);
}

auto CompilationScope::endScope() -> void {
    m_scopeDepth--;
    m_localScope = m_localScope->enclosing().value();
}

auto CompilationScope::addUpvalue(uint8_t index, bool isLocal) -> uint8_t {
    Upvalue * upvalue = nullptr;
    uint8_t upvalueCount = m_function->upvalueCount();
    for (auto i : std::views::iota(0u, upvalueCount) | std::views::reverse) {
        upvalue = &m_upvalues[i];
        if (upvalue->index() == index && upvalue->isLocal() == isLocal) {
            return i;
        }
    }
    m_upvalues[upvalueCount] = Upvalue(index, isLocal);
    m_function->incrementUpvalueCount();
    return upvalueCount;
}

auto CompilationScope::upvalue(uint8_t index) -> Upvalue & {
    return m_upvalues[index];
}
