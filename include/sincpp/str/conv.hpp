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

#ifndef SINCPP_STR_CONV_HPP
#define SINCPP_STR_CONV_HPP

#include <array>
#include <charconv>
#include <climits>
#include <cstddef>
#include <limits>

namespace sincpp {

// to_chars_max_size

/**
 * @brief Determines the maximum size required for converting an integer to a
 * string using `std::to_chars`.
 *
 * @tparam T Number type.
 *
 * @return the maximum size required for converting an integer to a string.
 *
 * **Example**:
 * @include examples/str/to_chars.cpp
 *
 * @ingroup sincpp_str
 */
template <class T>
  requires(std::is_integral_v<T>)
constexpr size_t to_chars_max_size() {
  return std::numeric_limits<T>::digits10 +
         (std::numeric_limits<T>::is_signed ? 2 : 1);
}

/**
 * @brief Determines the maximum size required for converting a floating-point
 * to a string using `std::to_chars`.
 *
 * @tparam FloatFormat Floating-point formatting.
 * @tparam T Number type.
 *
 * @todo Some values are empiric, it should be computed using formulas.
 *
 * @return the maximum size required for converting a floating-point to a
 * string.
 *
 * **Example**:
 * @include examples/str/to_chars.cpp
 *
 * @ingroup sincpp_str
 */
template <class T, std::chars_format FloatFormat = std::chars_format::general>
  requires(std::is_floating_point_v<T>)
constexpr size_t to_chars_max_size() {
  if constexpr (FloatFormat == std::chars_format::general ||
                FloatFormat == std::chars_format::scientific) {
    if constexpr (std::is_same_v<T, float>) {
      return 14;
    } else if constexpr (std::is_same_v<T, double>) {
      return 24;
    } else {
      return 32;
    }
  } else if constexpr (FloatFormat == std::chars_format::fixed) {
    return std::numeric_limits<T>::max_exponent10 + 1 +
           std::numeric_limits<T>::max_digits10 + 1;
  } else if constexpr (FloatFormat == std::chars_format::hex) {
    if constexpr (std::is_same_v<T, float>) {
      return 14;
    } else if constexpr (std::is_same_v<T, double>) {
      return 22;
    } else {
      return 30;
    }
  }
}

// to_chars

/**
 * @brief Functor to write a number to an already allocated string using
 * `std::to_chars`.
 *
 * @tparam FloatFormat Floating-point formatting.
 *
 * **Example**:
 * @include examples/str/to_chars.cpp
 *
 * @ingroup sincpp_str
 */
template <std::chars_format FloatFormat = std::chars_format::general>
class to_chars_t {
public:
  /**
   * @brief Writes a number to an already allocated string using
   * `std::to_chars`.
   *
   * @tparam T Number type.
   * @tparam N Output maximum size.
   *
   * @param output Output.
   * @param t Number.
   *
   * @return the number of characters written to the output and the `std::errc`.
   */
  template <class T, size_t N>
    requires(std::is_arithmetic_v<T>)
  std::to_chars_result operator()(std::array<char, N> &output, T const &t) {
    return to_chars(output.data(), output.data() + N, t);
  }

private:
  /**
   * @brief Call `std::to_chars` for integers.
   *
   * @param first Output beginning.
   * @param last Output ending.
   * @param value Integer.
   *
   * @return the result of `std::to_chars`.
   */
  static std::to_chars_result to_chars(char *const first, char *const last,
                                       std::integral auto const value) {
    return std::to_chars(first, last, value);
  }

  /**
   * @brief Call `std::to_chars` for floating-points.
   *
   * @param first Output beginning.
   * @param last Output ending.
   * @param value Floating-point.
   *
   * @return the result of `std::to_chars`.
   */
  static std::to_chars_result to_chars(char *const first, char *const last,
                                       std::floating_point auto const value) {
    return std::to_chars(first, last, value, FloatFormat);
  }
};

/**
 * @brief Writes a number to an already allocated string.
 *
 * @tparam FloatFormat Floating-point formatting.
 * @tparam T Number type.
 * @tparam N Output maximum size.
 *
 * @param output Output.
 * @param t Number.
 *
 * @return the number of characters written to the output and the `std::errc`.
 *
 * **Example**:
 * @include examples/str/to_chars.cpp
 *
 * @ingroup sincpp_str
 */
template <std::chars_format FloatFormat = std::chars_format::general, class T,
          size_t N>
  requires(std::is_arithmetic_v<T>)
std::to_chars_result to_chars(std::array<char, N> &output, T const &t) {
  return to_chars_t<FloatFormat>()(output, t);
}

} // namespace sincpp

#endif
