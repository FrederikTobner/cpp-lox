#pragma once

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <string>

#include <cstdint>

#include "../error/compiletime_exception.hpp"
#include "local.hpp"

namespace cppLox::Frontend {

/// @brief The compilation scope.
class CompilationScope {
  public:
    /// @brief Trivial constructor of the compilation scope.
    CompilationScope();

    /// @brief Constructor of the compilation scope.
    /// @param enclosing The enclosing compilation scope.
    CompilationScope(std::shared_ptr<CompilationScope> enclosing);

    /// @brief Destructor of the compilation scope.
    ~CompilationScope() = default;

    /// @brief Copy assignment operator of the compilation scope.
    CompilationScope & operator=(CompilationScope const &) {
        return *this;
    };

    /// @brief Gets the enclosing compilation scope.
    /// @return The enclosing compiler context.
    [[nodiscard]] auto enclosing() const -> std::optional<std::shared_ptr<CompilationScope>> const &;

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
    std::optional<std::shared_ptr<CompilationScope>> m_enclosing;
    /// The local variables.
    std::array<std::unique_ptr<cppLox::Frontend::Local>, 256> m_locals;
    /// The number of local variables.
    uint16_t m_localCount;
};

} // namespace cppLox::Frontend