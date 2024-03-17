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
 * @file object_function.hpp
 * @brief This file contains the definition of the ObjectFunction class.
 */

#pragma once

#include <memory>

#include "../bytecode/chunk.hpp"
#include "object.hpp"
#include "object_string.hpp"

namespace cppLox::Types {

/// @brief Represents a callable function object.
class ObjectFunction : public Object {
  public:
    /// @brief Constructor of the ObjectFunction.
    ObjectFunction(uint16_t arity, ObjectString * name);

    /// @brief Destructor of the ObjectFunction.
    ~ObjectFunction() override = default;

    /// @brief Copy constructor of the ObjectFunction.
    /// @param other The ObjectFunction to copy.
    /// @return The new ObjectFunction.
    auto operator=(ObjectFunction const & other) -> ObjectFunction &;

    /// @brief Gets the arity of the function.
    /// @return The arity of the function.
    [[nodiscard]] auto arity() const -> uint16_t;

    /// @brief Gets the chunk of the function.
    /// @return The chunk of the function.
    [[nodiscard]] auto chunk() -> cppLox::ByteCode::Chunk *;

    /// @brief Gets the name of the function.
    /// @return The name of the function.
    [[nodiscard]] auto name() const -> ObjectString *;

    /// @brief Writes the function to the output stream.
    /// @param os The output stream to write to.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << (m_name != nullptr ? "<fn " + m_name->string() + ">" : "<script>");
    }

    /// @brief Increments the arity of the function.
    auto incrementArity() -> void;

  private:
    /// @brief The arity of the function.
    uint16_t m_arity;

    /// @brief The chunk of the function.
    std::unique_ptr<cppLox::ByteCode::Chunk> m_chunk;

    /// @brief The name of the function.
    ObjectString * m_name;
};

} // namespace cppLox::Types