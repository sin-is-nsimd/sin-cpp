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
 * Convert types, if necessary, to simplify the JSON conversion.
 *
 * @tparam T Type of the value to convert.
 *
 * **Example:**
 * @include examples/json/to_json_value.cpp
 *
 * @ingroup sincpp_json
 */
template <class T> class to_json_value_t;

/// @brief Specialization for strings and chars.
/// @tparam S String or char type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class S>
  requires(sincpp::is_string_v<S> || sincpp::is_char_v<S>)
class to_json_value_t<S> {
public:
  /// @brief Get the JSON value of a string or a char.
  /// @details Leading and trailing quotes (`"`) are not added and no character
  /// is escaped.
  /// @param s A string or a char.
  /// @returns the JSON value of the string.
  S const &operator()(S const &s) { return s; }
};

/// @brief Specialization for bools.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <> class to_json_value_t<bool> {
public:
  /// @brief Get the JSON value of a bool.
  /// @param b A bool.
  /// @returns the JSON value of the number.
  bool operator()(bool const b) { return b; }
};

/// @brief Specialization for numbers (integer or floating-point).
/// @tparam N Number type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class N>
  requires(std::is_arithmetic_v<N> && sincpp::is_char_v<N> == false)
class to_json_value_t<N> {
public:
  /// @brief Get the JSON value of a number (integer or floating-point).
  /// @param t A number.
  /// @returns the JSON value of the number.
  sincpp::number_str_t<sincpp::to_chars_max_size<N>()> operator()(N const n) {
    return sincpp::number_str_t<sincpp::to_chars_max_size<N>()>(n);
  }
};

/// @brief Specialization for arrays of strings or chars.
/// @tparam S String or char type.
/// @tparam N Array size.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class S, size_t N>
  requires(sincpp::is_string_v<S> || sincpp::is_char_v<S>)
class to_json_value_t<std::array<S, N>> {
public:
  /// @brief Get the JSON value of an array of strings or chars.
  /// @param a An array of strings or chars.
  /// @returns the JSON value of the array of strings.
  // template <class A>
  std::array<S, N> const &operator()(std::array<S, N> const &a) { return a; }
};

/// @brief Specialization for arrays of numbers (integer or floating-point).
/// @tparam T Number type.
/// @tparam N Array size.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class T, size_t N>
  requires(std::is_arithmetic_v<T>)
class to_json_value_t<std::array<T, N>> {
public:
  /// @brief Get the JSON value of an array of numbers (integer or
  /// floating-point).
  /// @param a An array of numbers.
  /// @returns the JSON value of the array of numbers.
  std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N>
  operator()(std::array<T, N> const &a) {
    std::array<number_str_t<sincpp::to_chars_max_size<T>()>, N> r;
    std::ranges::transform(
        a, r.begin(), [](T const v) { return sincpp::make_number_str(v); });
    return r;
  }
};

/// @brief Specialization for C arrays of strings.
/// @tparam S String type.
/// @tparam N Array size.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class S, int N>
  requires(sincpp::is_string_v<S>)
class to_json_value_t<S[N]> {
public:
  /// @brief Get the JSON value of a C array of strings.
  /// @param a A C array of strings.
  /// @returns the JSON value of the C array of strings.
  std::array<std::string_view, N> operator()(S const (&a)[N]) {
    std::array<std::string_view, N> r;
    std::ranges::copy(a, a + N, r.begin());
    return r;
  }
};

/// @brief Specialization for C arrays of numbers (integer or floating-point).
/// @tparam T Number type.
/// @tparam N Array size.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class T, size_t N>
  requires(std::is_arithmetic_v<T> && sincpp::is_char_v<T> == false)
class to_json_value_t<T[N]> {
public:
  /// @brief Get the JSON value of a C array of numbers (integer or
  /// floating-point).
  /// @param a A C array of numbers.
  /// @returns the JSON value of the C array of numbers.
  std::array<number_str_t<sincpp::to_chars_max_size<T>()>, N>
  operator()(T const (&a)[N]) {
    std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N> r;
    std::transform(a, a + N, r.begin(),
                   [](T const v) { return sincpp::make_number_str(v); });
    return r;
  }
};

/// @brief Specialization for an 1D container of strings or chars.
/// @tparam CS Container type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class CS>
  requires(sincpp::is_container_1d_v<CS> &&
           (sincpp::is_string_v<typename CS::value_type> ||
            sincpp::is_char_v<typename CS::value_type>))
class to_json_value_t<CS> {
public:
  /// @brief Get the JSON value of an 1D container of strings or chars.
  /// @param c An 1D container of strings or chars.
  /// @returns the JSON value of the 1D container of strings or chars.
  CS const &operator()(CS const &c) { return c; }
};

/// @brief Specialization for an 1D container of numbers (integer or
/// floating-point).
/// @tparam C Container type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class C>
  requires(sincpp::is_container_1d_v<C> &&
           std::is_arithmetic_v<typename C::value_type>)
class to_json_value_t<C> {
public:
  /// @brief Get the JSON value of an 1D container of numbers (integer or
  /// floating-point).
  /// @param c An 1D container of numbers.
  /// @returns the JSON value of the 1D container of numbers.
  std::vector<
      sincpp::number_str_t<sincpp::to_chars_max_size<typename C::value_type>()>>
  operator()(C const &c) {
    std::vector<sincpp::number_str_t<
        sincpp::to_chars_max_size<typename C::value_type>()>>
        r(c.size());
    std::ranges::transform(c, r.begin(), [](typename C::value_type const v) {
      return make_number_str(v);
    });
    return r;
  }
};

/*template <class C>
  requires(sincpp::is_container_1d_v<C> &&
           (sincpp::is_string_v<typename C::value_type> == false &&
            sincpp::is_char_v<typename C::value_type> == false))
class to_json_value_t<C> {
public:
  // 1D container

  /// @brief Get the JSON value of an 1D container of numbers.
  /// @tparam T Number type.
  /// @tparam C 1D container of numbers type.
  /// @param c An 1D container of numbers.
  /// @returns the JSON value of the 1D container of numbers.
  std::vector<decltype(sincpp::to_json_value_t<typename C::value_type>{}(
      std::declval<typename C::value_type const &>()))>
  operator()(C const &c) {
    std::vector<decltype(sincpp::to_json_value_t<typename C::value_type>{}(
        std::declval<typename C::value_type const &>()))>
        r(c.size());
    std::ranges::transform(c, r.begin(), [](typename C::value_type const &v) {
      return sincpp::to_json_value_t<typename C::value_type>{}(v);
    });
    return r;
  }
};*/

/// @brief Specialization for a key-value container where keys and values are
/// strings or chars.
/// @tparam CS Container type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class CS>
  requires(is_container_key_value_v<CS> &&
           (sincpp::is_string_v<typename CS::key_type> ||
            sincpp::is_char_v<typename CS::key_type>) &&
           (sincpp::is_string_v<typename CS::mapped_type> ||
            sincpp::is_char_v<typename CS::mapped_type>))
class to_json_value_t<CS> {
public:
  /// @brief Get the JSON value of a key-value container where keys and values
  /// are strings or chars.
  /// @param c A Key-value container where keys and values are strings or chars.
  /// @returns the JSON value of the key-value container.
  CS const &operator()(CS const &c) { return c; }
};

/// @brief Specialization for a key-value container where keys are string or
/// chars and the values are numbers (integer or floating-point).
/// @tparam C Container type.
/// @see sincpp::to_json_value_t
/// @ingroup sincpp_json
template <class C>
  requires(is_container_key_value_v<C> &&
           (sincpp::is_string_v<typename C::key_type> ||
            sincpp::is_char_v<typename C::key_type>) &&
           std::is_arithmetic_v<typename C::mapped_type>)
class to_json_value_t<C> {
public:
  /// @brief Get the JSON value of a key-value container where keys are string
  /// or chars and the values are numbers (integer or floating-point).
  /// @param c A Key-value container where keys are string or chars and the
  /// values are numbers
  /// @returns the JSON value of the key-value container.
  sincpp::vector_pair_t<std::string_view,
                        sincpp::number_str_t<sincpp::to_chars_max_size<
                            typename C::mapped_type>()>>
  operator()(C const &c) {
    sincpp::vector_pair_t<std::string_view,
                          sincpp::number_str_t<sincpp::to_chars_max_size<
                              typename C::mapped_type>()>>
        r;
    r.reserve(c.size());
    for (auto const &[key, value] : c) {
      r.emplace(std::string_view(key), sincpp::make_number_str(value));
    }
    return r;
  }
};

/**
 * @copydoc sincpp::to_json_value_t
 *
 * @param t Value to convert.
 *
 * @returns the JSON output.
 *
 * @ingroup sincpp_json
 */
template <class T>
auto to_json_value(T const &t) -> decltype(sincpp::to_json_value_t<T>{}(t)) {
  return sincpp::to_json_value_t<T>{}(t);
}

} // namespace sincpp

#endif
