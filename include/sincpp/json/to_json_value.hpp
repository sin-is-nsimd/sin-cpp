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
#include "../type_traits/is_container_1d.hpp"
#include "../type_traits/is_string.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>
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

  /// @brief Operator `<<` between a `std::ostream` and a number.
  /// @param o A `std::ostream`.
  /// @param n A number.
  friend std::ostream &operator<<(std::ostream &o, number_str_t const &n) {
    return o << n.to_json();
  }
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

// to_json_value: array

/**
 * @brief Get the JSON value of an array of strings.
 *
 * @tparam T String type.
 * @tparam N Array size.
 * @tparam A Array of strings type.
 *
 * @param a An array of strings.
 *
 * @returns the JSON value of the array of strings.
 */
template <class T, size_t N, class A>
  requires(is_string_v<T> || is_char_v<T>)
A const &array_to_json_value(A const &a) {
  return a;
}

/**
 * @brief Get the JSON value of an array of numbers.
 *
 * @tparam T String type.
 * @tparam N Array size.
 * @tparam A Array of numbers type.
 *
 * @param a An array of numbers.
 *
 * @returns the JSON value of the array of numbers.
 */
template <class T, size_t N, class A>
  requires(std::is_arithmetic_v<T>)
std::array<number_str_t<to_chars_max_size<T>()>, N>
array_to_json_value(A const &a) {
  std::array<number_str_t<to_chars_max_size<T>()>, N> r;
  std::ranges::transform(a, r.begin(),
                         [](T const v) { return make_number_str(v); });
  return r;
}

/**
 * @brief Get the JSON value of an array.
 *
 * @tparam T String type.
 * @tparam N Array size.
 *
 * @param a An array.
 *
 * @returns the JSON value of the array.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class T, size_t N>
auto to_json_value(std::array<T, N> const &a)
    -> decltype(array_to_json_value<T, N>(a)) {
  return array_to_json_value<T, N>(a);
}

// to_json_value: C array

/**
 * @brief Get the JSON value of a C array of numbers.
 *
 * @tparam T Number type.
 * @tparam N Array size.
 *
 * @param a A C array of numbers.
 *
 * @returns the JSON value of the C array of numbers.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class T, size_t N>
  requires(std::is_arithmetic_v<T> && std::is_same_v<T, char> == false)
std::array<number_str_t<to_chars_max_size<T>()>, N>
to_json_value(T const (&a)[N]) {
  std::array<number_str_t<to_chars_max_size<T>()>, N> r;
  std::transform(a, a + N, r.begin(),
                 [](T const v) { return make_number_str(v); });
  return r;
}

/**
 * @brief Get the JSON value of a C array of strings.
 *
 * @tparam T String type.
 * @tparam N Array size.
 *
 * @param a A C array of strings.
 *
 * @returns the JSON value of the C array of strings.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class T, int N>
  requires(is_string_v<T>)
std::array<std::string_view, N> to_json_value(T const (&a)[N]) {
  std::array<std::string_view, N> r;
  std::ranges::copy(a, a + N, r.begin());
  return r;
}

// to_json_value: 1D container

/**
 * @brief Get the JSON value of an 1D container of strings.
 *
 * @tparam T String type.
 * @tparam C 1D container of strings type.
 *
 * @param c An 1D container of strings.
 *
 * @returns the JSON value of the 1D container of strings.
 */
template <class T, class C>
  requires(is_string_v<T> || is_char_v<T>)
C const &container_1d_to_json_value(C const &c) {
  return c;
}

/**
 * @brief Get the JSON value of an 1D container of numbers.
 *
 * @tparam T Number type.
 * @tparam C 1D container of numbers type.
 *
 * @param c An 1D container of numbers.
 *
 * @returns the JSON value of the 1D container of numbers.
 */
template <class T, class C>
  requires(std::is_arithmetic_v<T>)
std::vector<number_str_t<to_chars_max_size<T>()>>
container_1d_to_json_value(C const &c) {
  std::vector<number_str_t<to_chars_max_size<T>()>> r(c.size());
  std::ranges::transform(c, r.begin(),
                         [](T const v) { return make_number_str(v); });
  return r;
}

/**
 * @brief Get the JSON value of an 1D container.
 *
 * @tparam C 1D container type.
 *
 * @param c An 1D container.
 *
 * @returns the JSON value of the 1D container.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
template <class C>
  requires(is_container_1d_v<C>)
auto to_json_value(C const &c)
    -> decltype(container_1d_to_json_value<typename C::value_type>(c)) {
  return container_1d_to_json_value<typename C::value_type>(c);
}

} // namespace sincpp

#endif
