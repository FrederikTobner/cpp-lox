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
 * @file local_scope.hpp
 * @brief This file contains the declaration of the LocalScope class.
 */

#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include "local.hpp"

namespace cppLox::Frontend {

/// @brief The compilation scope.
class LocalScope {
  public:
    /// @brief Trivial constructor of the compilation scope.
    LocalScope();

    /// @brief Constructor of the compilation scope.
    /// @param enclosing The enclosing compilation scope.
    LocalScope(std::shared_ptr<LocalScope> enclosing);

    /// @brief Destructor of the compilation scope.
    ~LocalScope() = default;

    /// @brief Copy assignment operator of the compilation scope.
    LocalScope & operator=(LocalScope const &) {
        return *this;
    };

    /// @brief Gets the enclosing compilation scope.
    /// @return The enclosing compiler context.
    [[nodiscard]] auto enclosing() const -> std::optional<std::shared_ptr<LocalScope>> const &;

    /// @brief Gets the local variable at the given index.
    /// @param index The index of the local variable.
    /// @return The local variable at the given index.
    [[nodiscard]] auto local(uint16_t index) const -> Local const &;

    /// @brief Gets the number of local variables.
    /// @return The number of local variables.
    [[nodiscard]] auto localCount() const -> uint16_t const &;

    /// @brief Adds a local variable to the compilation scope.
    /// @param name The name of the local variable.
    /// @param depth The depth of the local variable.
    /// @param onError The error handler.
    /// @return The index of the local variable.
    auto addLocal(cppLox::Frontend::Token name, std::function<void(std::string &)> onError) -> void;

    /// @brief Marks the local variable at the given depth as initialized.
    /// @param depth The depth of the local variable.
    /// @return The index of the local variable.
    auto markInitialized(uint16_t depth) -> void;

    /// @brief Pops the local variables from the compilation scope.
    /// @return The number of local variables popped.
    auto popLocal() -> void;

  private:
    /// The enclosing compiler context.
    std::optional<std::shared_ptr<LocalScope>> m_enclosing;
    /// The local variables.
    std::array<std::unique_ptr<cppLox::Frontend::Local>, 256> m_locals;
    /// The number of local variables.
    uint16_t m_localCount;
};

} // namespace cppLox::Frontend