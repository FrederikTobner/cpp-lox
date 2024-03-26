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
 * @file object_native_fuction.hpp
 * @brief This file contains the declaration of the ObjectNativeFunction class.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "../backend/callframe.hpp"
#include "object.hpp"
#include "value.hpp"

namespace cppLox::Types {

/// @brief The Object Native Function class.
class ObjectNativeFunction : public Object {
  public:
    /// @brief Construct a new Object Native Function.
    /// @param function The function to call.
    /// @param arity The number of arguments the function takes.
    ObjectNativeFunction(
        std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                            std::function<void(cppLox::Backend::CallFrame & frame, std::string_view format)> onError)>
            function,
        int16_t arity);

    /// @brief Destructor for the Object Native Function class.
    ~ObjectNativeFunction() = default;

    /// @brief Writes the native function to the output stream.
    /// @param os The output stream to write to.
    /// @return The output stream.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << "<native fn>";
    }

    /// @brief Calls the underlying function.
    /// @param argCount The number of arguments passed to the function.
    /// @param args The arguments passed to the function.
    /// @param frame The call frame from which the function was called.
    /// @param onError The error handler.
    /// @return The result of the function call.
    auto call(uint8_t argCount, Value * args, cppLox::Backend::CallFrame & frame,
              std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError) -> Value;

    auto arity() const -> int16_t;

  private:
    /// @brief The function to call.
    std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                        std::function<void(cppLox::Backend::CallFrame & frame, std::string_view format)> onError)>
        m_function;

    /// @brief The number of arguments the function takes.
    int16_t m_arity;
};

} // namespace cppLox::Types