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

#ifndef SINCPP_TYPE_TRAITS_IS_STRING_HPP
#define SINCPP_TYPE_TRAITS_IS_STRING_HPP

#include "is_char.hpp"

#include <string>
#include <string_view>

namespace sincpp {

/**
 * @brief Type trait to check if a type is a string type.
 *
 * `std::string`, `std::string_view`, `char const *`, `char array[N]` are
 * strings.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_string.cpp
 *
 * @ingroup sincpp_type_traits
 */
template <class T> struct is_string : std::false_type {};

/// @brief Template specialization for `std::string`.
/// @relates sincpp::is_string
template <class T>
  requires(is_char_v<T>)
struct is_string<std::basic_string<T>> : std::true_type {};

/// @brief Template specialization for `std::string_view`.
/// @relates sincpp::is_string
template <class T>
  requires(is_char_v<T>)
struct is_string<std::basic_string_view<T>> : std::true_type {};

/// @brief Template specialization for `char *`.
/// @relates sincpp::is_string
template <class T>
  requires(is_char_v<T>)
struct is_string<T *> : std::true_type {};

/// @brief Template specialization for `char const *`.
/// @relates sincpp::is_string
template <class T>
  requires(is_char_v<T>)
struct is_string<T const *> : std::true_type {};

/// @brief Template specialization for C array.
/// @relates sincpp::is_string
template <class T, size_t N>
  requires(is_char_v<T>)
struct is_string<T[N]> : std::true_type {};

/**
 * @brief Variable template to check if a type is a string type.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_string.cpp
 *
 * @relates sincpp::is_string
 */
template <class T> constexpr bool is_string_v = is_string<T>::value;

} // namespace sincpp

#endif
