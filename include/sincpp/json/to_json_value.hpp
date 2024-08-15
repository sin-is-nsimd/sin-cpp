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

#ifndef SINCPP_JSON_TO_JSON_VALUE_HPP
#define SINCPP_JSON_TO_JSON_VALUE_HPP

#include "../str/conv.hpp"
#include "../type_traits/is_char.hpp"
#include "../type_traits/is_string.hpp"

#include <string_view>
#include <type_traits>

namespace sincpp {

// number_str_t

/**
 * @brief String representation of a number (integer or floating-point).
 *
 * @tparam N Maximum size of the string representation.
 *
 * **Example:**
 * @include examples/json/number_str.cpp
 *
 * @ingroup sincpp_json
 */
template <size_t N> class number_str_t {
private:
  /// @brief String representation of the number.
  std::array<char, N> m_str;

  /// @brief Size of the string representation.
  size_t m_size = 0;

public:
  /// @brief Default constructor.
  number_str_t() = default;

  /// @brief Constructor from a number.
  /// @tparam T Number type.
  /// @param t A number.
  /// @pre The size of the string representation of the number is less than or
  /// equal to the maximum size of string representation.
  template <class T>
    requires(std::is_arithmetic_v<T>)
  number_str_t(T const &t) : m_size(to_chars(m_str, t).ptr - m_str.data()) {}

  /// @brief Get the size of the JSON representation.
  /// @returns the size of the JSON representation.
  size_t to_json_size() const { return m_size; }

  /// @brief Get the JSON representation.
  /// @returns the JSON representation.
  std::string_view to_json() const { return {m_str.data(), m_size}; }
};

/**
 * @brief Create a sincpp::number_str_t from a number (integer or
 * floating-point).
 *
 * @tparam T Number type.
 * @tparam N Maximum size of the string representation.
 *
 * @param t A number.
 *
 * @pre The size of the string representation of the number is less than or
 * equal to the maximum size of string representation.
 *
 * **Example:**
 * @include examples/json/number_str.cpp
 *
 * @relates number_str_t
 */
template <class T, size_t N = to_chars_max_size<T>()>
  requires(std::is_arithmetic_v<T>)
number_str_t<N> make_number_str(T const t) {
  return number_str_t<N>(t);
}

// to_json_value: string & number

/**
 * @brief Get the JSON value of a string.
 *
 * Leading and trailing quotes (`"`) are not added and no character is escaped.
 *
 * @tparam T String type.
 *
 * @param t A string.
 *
 * @returns the JSON value of the string.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class T>
  requires(is_string_v<T> || is_char_v<T>)
T const &to_json_value(T const &t) {
  return t;
}

/**
 * @brief Get the JSON value of a number (integer or floating-point).
 *
 * @tparam T Integer or floating-point type.
 *
 * @param t A number.
 *
 * @returns the JSON value of the number.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class T>
  requires(std::is_arithmetic_v<T> && is_char_v<T> == false)
number_str_t<sincpp::to_chars_max_size<T>()> to_json_value(T const t) {
  return number_str_t<sincpp::to_chars_max_size<T>()>(t);
}

} // namespace sincpp

#endif
