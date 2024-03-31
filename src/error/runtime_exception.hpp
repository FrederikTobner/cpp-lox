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
 * @file runtime_exception.hpp
 * @brief This file contains the declaration of the RunTimeException class.
 */

#pragma once

#include <exception>
#include <string>
#include <string_view>

namespace cppLox::Error {

/// @brief Exception that is thrown when an error occurs during run time.
class RunTimeException : public std::exception {
  public:
    /// @brief The constructor of the RunTimeException class.
    /// @param message The underlying message of the exception.
    RunTimeException(std::string_view message);

    /// @brief Gets the message of the exception.
    /// @return The underlying message of the exception.
    [[nodiscard]] char const * what() const noexcept override;

  private:
    /// @brief The underlying message of the exception.
    std::string m_message;
};

} // namespace cppLox::Error