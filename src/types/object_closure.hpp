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
 * @file object_closure.hpp
 * @brief This file contains the definition of the ObjectClosure class.
 */

#pragma once

#include "object.hpp"
#include "object_function.hpp"
#include "object_upvalue.hpp"

#include <vector>

namespace cppLox::Types {

/// @brief Represents a closure object.
class ObjectClosure : public Object {
  public:
    /// @brief Constructor of the ObjectClosure.
    /// @param function The function of the closure.
    ObjectClosure(ObjectFunction * function);

    /// @brief Destructor of the ObjectClosure.
    ~ObjectClosure() override = default;

    /// @brief Gets the function of the closure.
    /// @return The function of the closure.
    [[nodiscard]] auto function() const noexcept -> ObjectFunction *;

    /// @brief Gets the upvalues of the closure.
    /// @return The upvalues of the closure.
    [[nodiscard]] auto upvalues() noexcept -> std::vector<ObjectUpValue *> &;

    /// @brief Writes the closure to the output stream.
    /// @param os The output stream to write to.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << (m_function->name() != nullptr ? "<fn " + m_function->name()->string() + ">" : "<script>");
    }

  private:
    /// @brief The function of the closure.
    ObjectFunction * m_function;
    /// @brief The upvalues of the closure.
    std::vector<ObjectUpValue *> m_upvalues;
};

} // namespace cppLox::Types