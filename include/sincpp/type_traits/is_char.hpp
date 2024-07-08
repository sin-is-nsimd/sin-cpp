// Copyright © 2024 Lénaïc Bagnères, lenaicb@singularity.fr

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SINCPP_TYPE_TRAITS_IS_CHAR_HPP
#define SINCPP_TYPE_TRAITS_IS_CHAR_HPP

#include <string>
#include <type_traits>

namespace sincpp {

/**
 * @brief Type trait to check if a type is a character type.
 *
 * `char`, `wchar_t`, `char8_t`, `char16_t` and `char32_t` are character types.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_char.cpp
 *
 * @ingroup sincpp_type_traits
 */
template <class T> struct is_char : std::false_type {};

/// @brief Template specialization for `char`.
/// @relates sincpp::is_char
template <> struct is_char<char> : std::true_type {};

/// @brief Template specialization for `wchar_t`.
/// @relates sincpp::is_char
template <> struct is_char<wchar_t> : std::true_type {};

/// @brief Template specialization for `char8_t`.
/// @relates sincpp::is_char
template <> struct is_char<char8_t> : std::true_type {};

/// @brief Template specialization for `char16_t`.
/// @relates sincpp::is_char
template <> struct is_char<char16_t> : std::true_type {};

/// @brief Template specialization for `char32_t`.
/// @relates sincpp::is_char
template <> struct is_char<char32_t> : std::true_type {};

/**
 * @brief Variable template to check if a type is a character type.
 *
 * `char`, `wchar_t`, `char8_t`, `char16_t` and `char32_t` are character types.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_char.cpp
 *
 * @relates sincpp::is_char
 */
template <class T> constexpr bool is_char_v = is_char<T>::value;

} // namespace sincpp

#endif
