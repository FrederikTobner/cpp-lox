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
 * @file parser_traits.hpp
 * @brief This file contains the type traits for Pratt parsers.
 */

#pragma once

#include <concepts>

namespace cppLox::Traits {

/// @brief Used to check if a type has a specific method signature.
/// @tparam T The type to check.
template <typename T> struct MemberFunctionTraits;

/// @brief Used to check if a type has a specific method signature.
/// @tparam T The type to check.
/// @tparam R The return type of the method.
/// @tparam ...ARGS The arguments of the method.
template <typename T, typename R, typename... ARGS> struct MemberFunctionTraits<R (T::*)(ARGS...)> {
    using return_type = R;
    using args_tuple = std::tuple<ARGS...>;
    static constexpr size_t arity = sizeof...(ARGS);
};

/// @brief Used to check if a type has a specific method signature.
/// @tparam T The type to check.
/// @tparam R The return type of the method.
/// @tparam ...ARGS The arguments of the method.
template <typename T, typename R, typename... ARGS> struct MemberFunctionTraits<R (T::*)(ARGS...) const> {
    using return_type = R;
    using args_tuple = std::tuple<ARGS...>;
    static constexpr size_t arity = sizeof...(ARGS);
};

/// @brief Used to check if a type has a specific method signature.
/// @tparam EXPECTED_SIGNATURE The expected signature of the method.
/// @tparam METHOD The method to check.
template <auto METHOD, typename EXPECTED_SIGNATURE> struct MethodSignatureCheck {
    static constexpr auto method = METHOD;

    static_assert(
        requires { method; }, "Required method is missing");

    static_assert(std::is_member_function_pointer_v<decltype(METHOD)>, "Method must be a member function pointer");
    static_assert(std::is_member_function_pointer_v<EXPECTED_SIGNATURE>,
                  "Expected signature must be a member function pointer");

    static_assert(std::is_same_v<decltype(METHOD), EXPECTED_SIGNATURE>,
                  "Method signature does not match expected signature");

    using actual_traits = MemberFunctionTraits<decltype(METHOD)>;
    using expected_traits = MemberFunctionTraits<EXPECTED_SIGNATURE>;

    static_assert(std::is_same_v<typename actual_traits::return_type, typename expected_traits::return_type>,
                  "Return type mismatch");
    static_assert(std::is_same_v<typename actual_traits::args_tuple, typename expected_traits::args_tuple>,
                  "Parameter types mismatch");

    static_assert(actual_traits::arity == expected_traits::arity, "Arity mismatch");

    static constexpr bool allChecksSucceded = std::is_same_v<decltype(METHOD), EXPECTED_SIGNATURE>;
};

/// @brief Used to check if a type has a specific method signature.
/// @tparam PARSER The type to check.
/// @tparam ...CHECKS The checks to perform.
template <typename PARSER, typename... CHECKS> struct MethodSignatureChecks {
    template <typename CHECK> static constexpr bool verifyMethod() {
        static_assert(
            requires { CHECK::allChecksSucceded; }, "Method has invalid signature");

        return true;
    }

    static constexpr bool checkMethods() {
        return (verifyMethod<CHECKS>() && ...);
    }

    static constexpr bool hasRequiredMethods = checkMethods();
};

} // namespace cppLox::Traits
