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
 * @file object_upvalue.hpp
 * @brief This file contains the definition of the ObjectUpValue class.
 */

#pragma once

#include "../language_features.hpp"
#include "object.hpp"
#include "value.hpp"


namespace cppLox::Types {

/// @brief Represents an upvalue object.
class ObjectUpValue : public Object {
  public:
    /// @brief Constructs a new upvalue object.
    /// @param location The location of the value that is captured, either a slot on the stack or, once closed, the
    /// internal storage of the upvalue itself.
    ObjectUpValue(Value * location);

    /// @brief Destructor of the upvalue object.
    ~ObjectUpValue() override = default;

    /// @brief Gets the location of the value that is captured by the upvalue.
    /// @return The location of the value.
    [[nodiscard]] auto location() const _NO_EXCEPT->Value *;

    /// @brief Sets the location of the value that is captured by the upvalue.
    /// @param location The new location of the value.
    auto setLocation(Value * location) -> void;

    /// @brief Whether the upvalue has been closed, meaning it no longer points into the stack.
    /// @return Whether the upvalue has been closed.
    [[nodiscard]] auto isClosed() const _NO_EXCEPT->bool;

    /// @brief Closes the upvalue, copying the value it currently points to into the upvalue itself and updating its
    /// location to point at that internal copy. This is used when the value the upvalue refers to would otherwise go
    /// out of scope, e.g. when the enclosing function returns or the enclosing block ends.
    auto close() -> void;

    /// @brief Writes the upvalue to the output stream.
    /// @param os The output stream to write to.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << "upvalue";
    }

  private:
    /// @brief The location of the value that is captured by the upvalue.
    Value * m_location;

    /// @brief The value that is stored once the upvalue is closed.
    Value m_closedValue;
};

} // namespace cppLox::Types