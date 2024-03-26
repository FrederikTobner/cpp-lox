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
 * @file object_string.hpp
 * @brief This file contains the declaration of the ObjectString class.
 */

#pragma once

#include <memory>
#include <string>

#include "object.hpp"

namespace cppLox::Types {

/// @brief Represents a string object.
class ObjectString : public Object {
  public:
    /// @brief Constructs a new string object.
    /// @param value The value of the string object.
    ObjectString(std::string const & string);

    /// @brief Destructor of the string object
    ~ObjectString() override = default;

    /// @brief Gets the value of the string object.
    /// @return The value of the string object.
    [[nodiscard]] auto string() const -> std::string const &;

    /// @brief Checks if the string is equal to the given string.
    /// @param other The string to compare against.
    /// @return True if the strings are equal, false otherwise.
    auto operator==(ObjectString const & other) const -> bool;

    /// @brief Checks if the string is equal to the given string.
    /// @param other The string to compare against.
    /// @return True if the strings are equal, false otherwise.
    auto operator==(ObjectString const * other) const -> bool;

    /// @brief Writes the string to the output stream.
    /// @param os The output stream to write to.
    /// @return The output stream.
    virtual auto writeToOutputStream(std::ostream & os) const -> void override {
        os << *m_string.get();
    }

  private:
    /// @brief The value of the underlying string.
    std::unique_ptr<std::string> m_string;
};

} // namespace cppLox::Types