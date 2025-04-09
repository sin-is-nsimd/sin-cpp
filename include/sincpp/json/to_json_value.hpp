// Copyright © 2024-2025 Lénaïc Bagnères, lenaicb@singularity.fr

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
#include "../type_traits/is_container_key_value.hpp"
#include "../type_traits/is_string.hpp"

#include <algorithm>
#include <string_view>

namespace sincpp {

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
  std::array<char, N> _str;

  /// @brief Size of the string representation.
  size_t _size{0};

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
  explicit number_str_t(T const &t)
      : _size{size_t(sincpp::to_chars(_str, t).ptr - _str.data())} {}

  /// @brief Get the size of the JSON representation.
  /// @returns the size of the JSON representation.
  size_t to_json_size() const { return _size; }

  /// @brief Get the JSON representation.
  /// @returns the JSON representation.
  std::string_view to_json() const { return {_str.data(), _size}; }

  /// @brief Operator `<<` between a `std::ostream` and a number.
  /// @param o A `std::ostream`.
  /// @param n A number.
  friend std::ostream &operator<<(std::ostream &o, number_str_t const &n) {
    return o << n.to_json();
  }

  /// @brief Equality comparison between two sincpp::number_str_t.
  /// @returns true if the numbers have the same value, false otherwise.
  bool operator==(number_str_t const &n) const {
    if (_size != n._size) {
      return false;
    }
    for (size_t i = 0; i < _size; ++i) {
      if (_str[i] != n._str[i]) {
        return false;
      }
    }
    return true;
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
  return sincpp::number_str_t<N>(t);
}

/**
 * @brief Get the JSON value.
 *
 * Convert types to simplify the JSON conversion.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 */
class to_json_value_t {
public:
  // String

  /// @brief Get the JSON value of a string.
  /// @details Leading and trailing quotes (`"`) are not added and no character
  /// is escaped.
  /// @tparam T String type.
  /// @param t A string.
  /// @returns the JSON value of the string.
  template <class T>
    requires(sincpp::is_string_v<T> || sincpp::is_char_v<T>)
  T const &operator()(T const &t) {
    return t;
  }

  // Bool

  /// @brief Get the JSON value of a number (integer or floating-point).
  /// @tparam T Integer or floating-point type.
  /// @param t A number.
  /// @returns the JSON value of the number.
  bool operator()(bool const b) { return b; }

  // Number

  /// @brief Get the JSON value of a number (integer or floating-point).
  /// @tparam T Integer or floating-point type.
  /// @param t A number.
  /// @returns the JSON value of the number.
  template <class T>
    requires(std::is_arithmetic_v<T> && sincpp::is_char_v<T> == false)
  sincpp::number_str_t<sincpp::to_chars_max_size<T>()> operator()(T const t) {
    return sincpp::number_str_t<sincpp::to_chars_max_size<T>()>(t);
  }

  // Array

  /// @brief Get the JSON value of an array of strings.
  /// @tparam T String type.
  /// @tparam N Array size.
  /// @tparam A Array of strings type.
  /// @param a An array of strings.
  /// @returns the JSON value of the array of strings.
  template <class T, size_t N, class A>
    requires(sincpp::is_string_v<T> || sincpp::is_char_v<T>)
  A const &array_to_json_value(A const &a) {
    return a;
  }

  /// @brief Get the JSON value of an array of numbers.
  /// @tparam T String type.
  /// @tparam N Array size.
  /// @tparam A Array of numbers type.
  /// @param a An array of numbers.
  /// @returns the JSON value of the array of numbers.
  template <class T, size_t N, class A>
    requires(std::is_arithmetic_v<T>)
  std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N>
  array_to_json_value(A const &a) {
    std::array<number_str_t<sincpp::to_chars_max_size<T>()>, N> r;
    std::ranges::transform(
        a, r.begin(), [](T const v) { return sincpp::make_number_str(v); });
    return r;
  }

  /// @brief Get the JSON value of an array.
  /// @tparam T String type.
  /// @tparam N Array size.
  /// @param a An array.
  /// @returns the JSON value of the array.
  template <class T, size_t N>
  auto operator()(std::array<T, N> const &a)
      -> decltype(array_to_json_value<T, N>(a)) {
    return array_to_json_value<T, N>(a);
  }

  // C array

  /// @brief Get the JSON value of a C array of strings.
  /// @tparam T String type.
  /// @tparam N Array size.
  /// @param a A C array of strings.
  /// @returns the JSON value of the C array of strings.
  template <class T, int N>
    requires(sincpp::is_string_v<T>)
  std::array<std::string_view, N> operator()(T const (&a)[N]) {
    std::array<std::string_view, N> r;
    std::ranges::copy(a, a + N, r.begin());
    return r;
  }

  /// @brief Get the JSON value of a C array of numbers.
  /// @tparam T Number type.
  /// @tparam N Array size.
  /// @param a A C array of numbers.
  /// @returns the JSON value of the C array of numbers.
  template <class T, size_t N>
    requires(std::is_arithmetic_v<T> && std::is_same_v<T, char> == false)
  std::array<number_str_t<sincpp::to_chars_max_size<T>()>, N>
  operator()(T const (&a)[N]) {
    std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N> r;
    std::transform(a, a + N, r.begin(),
                   [](T const v) { return sincpp::make_number_str(v); });
    return r;
  }

  // 1D container

  /// @brief Get the JSON value of an 1D container of strings.
  /// @tparam T String type.
  /// @tparam C 1D container of strings type.
  /// @param c An 1D container of strings.
  /// @returns the JSON value of the 1D container of strings.
  template <class T, class C>
    requires(sincpp::is_string_v<T> || sincpp::is_char_v<T>)
  C const &container_1d_to_json_value(C const &c) {
    return c;
  }

  /// @brief Get the JSON value of an 1D container of numbers.
  /// @tparam T Number type.
  /// @tparam C 1D container of numbers type.
  /// @param c An 1D container of numbers.
  /// @returns the JSON value of the 1D container of numbers.
  template <class T, class C>
    requires(std::is_arithmetic_v<T>)
  std::vector<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>>
  container_1d_to_json_value(C const &c) {
    std::vector<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>> r(
        c.size());
    std::ranges::transform(c, r.begin(),
                           [](T const v) { return make_number_str(v); });
    return r;
  }

  /// @brief Get the JSON value of an 1D container.
  /// @tparam C 1D container type.
  /// @param c An 1D container.
  /// @returns the JSON value of the 1D container.
  template <class C>
    requires(sincpp::is_container_1d_v<C>)
  auto operator()(C const &c)
      -> decltype(container_1d_to_json_value<typename C::value_type>(c)) {
    return container_1d_to_json_value<typename C::value_type>(c);
  }

  // Key-value container

  /// @brief Get the JSON value of a key-value container (key and value are
  /// strings).
  /// @tparam C Key-value container type (key and value are strings).
  /// @param c A Key-value container (key and value are strings).
  /// @returns the JSON value of the key-value container (key and value are
  /// strings).
  template <class Key, class T, class C>
    requires((sincpp::is_string_v<Key> || sincpp::is_char_v<Key>) &&
             (sincpp::is_string_v<T> || sincpp::is_char_v<T>))
  C const &container_key_value_to_json_value(C const &c) {
    return c;
  }

  /// @brief Get the JSON value of a key-value container (key is a string, value
  /// is a number).
  /// @tparam C Key-value container type (key is a string, value is a number).
  /// @param c A Key-value container (key is a string, value is a number).
  /// @returns the JSON value of the key-value container (key is a string, value
  /// is a number).
  template <class Key, class T, class C>
    requires((sincpp::is_string_v<Key> || sincpp::is_char_v<Key>) &&
             std::is_arithmetic_v<T>)
  sincpp::vector_pair_t<std::string_view,
                        sincpp::number_str_t<sincpp::to_chars_max_size<T>()>>
  container_key_value_to_json_value(C const &c) {
    sincpp::vector_pair_t<std::string_view,
                          sincpp::number_str_t<sincpp::to_chars_max_size<T>()>>
        r;
    r.reserve(c.size());
    for (auto const &[key, value] : c) {
      r.emplace(std::string_view(key), sincpp::make_number_str(value));
    }
    return r;
  }

  /// @brief Get the JSON value of a key-value container (key is a string).
  /// @tparam C Key-value container type (key is a string).
  /// @param c A Key-value container (key is a string).
  /// @returns the JSON value of the key-value container (key is a string).
  template <class C>
    requires(is_container_key_value_v<C>)
  auto operator()(C const &c)
      -> decltype(container_key_value_to_json_value<
                  typename C::key_type, typename C::mapped_type>(c)) {
    return container_key_value_to_json_value<typename C::key_type,
                                             typename C::mapped_type>(c);
  }
};

/**
 * @copydoc sincpp::to_json_value_t
 *
 * @tparam T Type of the value to convert.
 *
 * @param t Value to convert.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 *
 * @returns the JSON output.
 */
template <class T>
auto to_json_value(T const &t) -> decltype(sincpp::to_json_value_t{}(t)) {
  return sincpp::to_json_value_t{}(t);
}

} // namespace sincpp

#endif
