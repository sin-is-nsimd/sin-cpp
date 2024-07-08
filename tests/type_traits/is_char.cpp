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

#include <sincpp/type_traits/is_char.hpp>

#include <gtest/gtest.h>

TEST(is_char, char) {
  static_assert(sincpp::is_char_v<char>);
  static_assert(sincpp::is_char_v<wchar_t>);
  static_assert(sincpp::is_char_v<char8_t>);
  static_assert(sincpp::is_char_v<char16_t>);
  static_assert(sincpp::is_char_v<char32_t>);
}

TEST(is_char, not_char) {
  static_assert(sincpp::is_char_v<int> == false);
  static_assert(sincpp::is_char_v<float> == false);
  static_assert(sincpp::is_char_v<double> == false);
  static_assert(sincpp::is_char_v<char const *> == false);
  static_assert(sincpp::is_char_v<std::string> == false);
}
