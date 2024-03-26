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
 * @file token.cpp
 * @brief This file contains the implementation of the Token class.
 */

#include "token.hpp"

#include <format>
#include <iostream>

using namespace cppLox::Frontend;

Token::Token(Token::Type type, std::string lexeme, std::size_t && line) : m_type(type), m_lexeme(lexeme), m_line(line) {
}
