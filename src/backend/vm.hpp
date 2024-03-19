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
 * @file vm.hpp
 * @details This file contains the declaration of the virtual machine used by the cpplox interpreter
 */

#pragma once

#include <array>
#include <cstdint>
#include <format>
#include <functional>

#include "../bytecode/chunk.hpp"
#include "../memory_mutator.hpp"
#include "../types/value.hpp"
#include "callframe.hpp"

class VMIntegrationTest;

namespace cppLox::Backend {

/// @brief The maximum amount of call frames that can be stored
#define FRAME_MAX 64

/// @brief The maximum amount of values that can be stored on the stack
#define STACK_MAX (FRAME_MAX * UINT8_MAX)

/// @brief The virtual machine used by the cpplox interpreter
class VM {
    friend class ::VMIntegrationTest;

  public:
    /// @brief Constructs a new virtual machine
    /// @param memoryMutator The memory mutator that is used by the virtual machine
    VM(std::shared_ptr<cppLox::MemoryMutator> memoryMutator);

    /// @brief Destructor of the virtual machine
    ~VM() = default;

    /// @brief Gets two bytes from the chunk and returns them as a 16-bit integer
    /// @param chunk The chunk to get the bytes from
    /// @param offset The offset to get the bytes from
    /// @return The 16-bit integer
    [[nodiscard]] auto getShort(CallFrame & frame) -> uint16_t;

    /// @brief Interprets the given chunk
    /// @param chunk The chunk to interpret
    auto interpret(cppLox::Types::ObjectFunction & function) -> void;

    /// @brief Peeks at the top value of the stack
    /// @return The top value of the stack
    [[nodiscard]] auto peek(CallFrame & frame) -> cppLox::Types::Value;

    /// @brief Pops the top value from the stack
    /// @return The top value of the stack
    auto pop(CallFrame & frame) -> cppLox::Types::Value;

    /// @brief Pushes the given value onto the stack
    /// @param value The value to push onto the stack
    auto push(CallFrame & frame, cppLox::Types::Value value) -> void;

    /// @brief Resets the stack
    auto resetStack() noexcept -> void;

  private:
    /// @brief Throws a runtime exception with the given message
    /// @tparam ...Args The types of the arguments
    /// @param fmt The format string
    /// @param ...args The arguments to the format string
    template <class... Args> auto runTimeError(CallFrame & frame, std::string_view fmt, Args &&... args) -> void;

    /// @brief Calls the given function with the given argument count
    /// @param value The value to call
    /// @param arg_count The amount of arguments to pass to the function
    /// @param frame The call frame
    auto callFunction(cppLox::Types::Value & value, uint8_t arg_count, CallFrame & frame) -> void;

    /// @brief Calls the given function with the given argument count
    /// @param function The function to call
    /// @param arg_count The amount of arguments to pass to the function
    auto call(cppLox::Types::ObjectFunction & function, uint8_t arg_count) -> void;

    /// @brief Runs the given function
    /// @param function The function to run
    auto run(cppLox::Types::ObjectFunction & function) -> void;

    /// @brief Defines a native function with the given name and function
    /// @tparam ARITY The arity of the function
    /// @param name The name of the function
    /// @param function The function that is wrapped in the native function object
    template <int16_t ARITY>
    auto defineNative(std::string const & name,
                      std::function<cppLox::Types::Value(int, cppLox::Types::Value *, CallFrame &,
                                                         std::function<void(CallFrame &, std::string_view fmt)>)>
                          function) -> void;

    /// @brief The stack
    cppLox::Types::Value m_stack[STACK_MAX];

    /// @brief The index of the top of the stack
    size_t m_stack_top;

    /// @brief The memory manager.
    std::shared_ptr<cppLox::MemoryMutator> m_memoryMutator;

    /// @brief The call frames
    std::array<CallFrame, FRAME_MAX> m_frames;

    /// @brief The count of the current call frame
    size_t m_frame_count;
};
} // namespace cppLox::Backend
