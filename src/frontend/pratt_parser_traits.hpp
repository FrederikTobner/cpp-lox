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
 * @file pratt_parser_traits.hpp
 * @brief This file contains the type traits for Pratt parsers.
 */

#pragma once

#include <concepts>

#include "../traits/method_signature_traits.hpp"

namespace cppLox::Frontend {

using Traits::MethodSignatureCheck;
using Traits::MethodSignatureChecks;

template <typename PARSER> struct Signatures {
    using PREFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &, bool);
    using INFIX_SIGNATURE = void (PARSER::*)(std::vector<Token> const &);
};

/// @brief Checks if the given type has the required methods for a Pratt parser.
/// @tparam PARSER The type to check.
template <typename PARSER> class PrattParserTrait {

  public:
    static constexpr bool hasRequiredMethods = MethodSignatureChecks<
        PARSER, MethodSignatureCheck<&PARSER::grouping, typename Signatures<PARSER>::PREFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::variable, typename Signatures<PARSER>::PREFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::string, typename Signatures<PARSER>::PREFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::number, typename Signatures<PARSER>::PREFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::call, typename Signatures<PARSER>::INFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::binary, typename Signatures<PARSER>::INFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::and_, typename Signatures<PARSER>::INFIX_SIGNATURE>,
        MethodSignatureCheck<&PARSER::or_, typename Signatures<PARSER>::INFIX_SIGNATURE>>::hasRequiredMethods;
};

} // namespace cppLox::Frontend
