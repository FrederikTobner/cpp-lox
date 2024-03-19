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
 * @file compilation_scope.hpp
 * @brief This file contains the declaration of the CompilationScope class.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "../types/object_function.hpp"
#include "function_type.hpp"
#include "local_scope.hpp"

namespace cppLox::Frontend {

/// @brief Models a compilation scope - meaning a scope in which a function is compiled.
class CompilationScope {
  public:
    /// @brief Constructs a new compilation scope.
    /// @param function The function to compile.
    /// @param type The type of the function.
    CompilationScope(cppLox::Types::ObjectFunction * function, FunctionType type);

    /// @brief Constructs a new compilation scope.
    /// @param enclosing The enclosing scope.
    /// @param function The function to compile.
    /// @param type The type of the function.
    CompilationScope(std::shared_ptr<CompilationScope> enclosing, cppLox::Types::ObjectFunction * function,
                     FunctionType type);

    /// @brief Destructs the compilation scope.
    ~CompilationScope() = default;

    /// @brief Gets the enclosing scope.
    /// @return The enclosing scope.
    [[nodiscard]] auto enclosing() const -> std::optional<std::shared_ptr<CompilationScope>> const &;

    /// @brief Gets the function to compile.
    /// @return The function to compile.
    [[nodiscard]] auto function() const -> cppLox::Types::ObjectFunction *;

    /// @brief Gets the type of the current function.
    /// @return The type of the current function.
    [[nodiscard]] auto currentFunctionType() const -> FunctionType const &;

    /// @brief Gets the scope depth.
    [[nodiscard]] auto scopeDepth() const -> uint16_t const &;

    /// @brief Gets the local scope.
    [[nodiscard]] auto localScope() const -> std::shared_ptr<LocalScope> const &;

    /// @brief Begins a new scope.
    auto beginNewScope() -> void;

    /// @brief Ends the current scope.
    auto endScope() -> void;

  private:
    /// @brief The enclosing scope.
    std::optional<std::shared_ptr<CompilationScope>> m_enclosing;

    /// @brief The function to compile.
    cppLox::Types::ObjectFunction * m_function;

    /// @brief The type of the current function.
    FunctionType m_currentFunctionType;

    /// The scope depth.
    uint16_t m_scopeDepth;

    /// @brief The current compiler context.
    std::shared_ptr<LocalScope> m_localScope;
};
} // namespace cppLox::Frontend