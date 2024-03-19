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

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "../backend/callframe.hpp"
#include "object.hpp"
#include "value.hpp"

namespace cppLox::Types {

class ObjectNativeFunction : public Object {
  public:
    ObjectNativeFunction(
        std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                            std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)>
            function,
        int16_t arity)
        : m_function(function), m_arity(arity) {
        m_type = Object::Type::NATIVE_FUNCTION;
    }

    ~ObjectNativeFunction() = default;

    /// @brief Writes the native function to the output stream.
    /// @param os The output stream to write to.
    /// @return The output stream.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << "<native fn>";
    }

    auto call(uint8_t argCount, Value * args, cppLox::Backend::CallFrame & frame,
              std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError) -> Value;

    auto arity() const -> int16_t;

  private:
    std::function<Value(int, Value *, cppLox::Backend::CallFrame & frame,
                        std::function<void(cppLox::Backend::CallFrame & frame, std::string_view fmt)> onError)>
        m_function;
    int16_t m_arity;
};

} // namespace cppLox::Types