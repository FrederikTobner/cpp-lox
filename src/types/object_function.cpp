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
 * @file object_function.cpp
 * @brief This file contains the implementation of the ObjectFunction class.
 */

#include "object_function.hpp"

using namespace cppLox::Types;

ObjectFunction::ObjectFunction(uint16_t arity, ObjectString * name) : m_arity(arity), m_name(name) {
    m_type = Object::Type::FUNCTION;
    m_chunk = std::make_unique<cppLox::ByteCode::Chunk>();
}

auto ObjectFunction::operator=(ObjectFunction const & other) -> ObjectFunction & {
    m_arity = other.m_arity;
    m_chunk = std::make_unique<cppLox::ByteCode::Chunk>(*other.m_chunk);
    m_name = other.m_name;
    return *this;
};

[[nodiscard]] auto ObjectFunction::arity() const -> uint16_t {
    return m_arity;
}

[[nodiscard]] auto ObjectFunction::chunk() -> cppLox::ByteCode::Chunk * {
    return m_chunk.get();
}

[[nodiscard]] auto ObjectFunction::name() const -> ObjectString * {
    return m_name;
}

auto ObjectFunction::incrementArity() -> void {
    m_arity++;
}