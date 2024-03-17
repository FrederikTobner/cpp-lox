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
 * @file object.hpp
 * @brief This file contains the definition of the Object class.
 */

#pragma once

#include <iostream>
#include <type_traits>

namespace cppLox::Types {

// Forward declaration of Object type.
class Object;

/// @brief Checks if the given type is derived from Object.
template <typename T>
concept DerivedFromObject = std::derived_from<T, cppLox::Types::Object>;

/// @brief The base class for all objects.
class Object {
  public:
    /// @brief The type of an object.
    enum class Type {
        /// @brief An ObjectFunction.
        FUNCTION,
        /// @brief An ObjectString.
        STRING
    };

    /// @brief Destructor of the object.
    virtual ~Object() = default;

    /// @brief Gets the type of the object.
    /// @return The type of the object.
    [[nodiscard]] auto type() const -> Object::Type;

    /// @brief Checks if the object is of the given type.
    /// @param type The type to check against.
    /// @return True if the object is of the given type, false otherwise.
    [[nodiscard]] auto is(Type type) const -> bool;

    /// @brief Casts the object to the given type.
    /// @tparam T The type to cast to.
    /// @return The object cast to the given type.
    template <DerivedFromObject T> [[nodiscard]] auto as() -> T * {
        return static_cast<T *>(this);
    }

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Object const & dt) -> std::ostream &;

    /// @brief Prints the value to the given output stream
    /// @param os The output stream to print to
    /// @param dt The value to print
    /// @return The output stream
    friend auto operator<<(std::ostream & os, Object * dt) -> std::ostream &;

    /// @brief Writes the value to the given output stream
    /// @param os The output stream to write to
    virtual void writeToOutputStream(std::ostream & os) const = 0;

  protected:
    /// @brief The type of the object.
    Type m_type;

    /// @brief Constructs a new object.
    Object() = default;
};
} // namespace cppLox::Types
