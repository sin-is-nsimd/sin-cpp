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

#include <sincpp/type_traits/is_string.hpp>

#include <gtest/gtest.h>

TEST(is_string, string) {
  static_assert(sincpp::is_string_v<std::string>);
  static_assert(sincpp::is_string_v<std::wstring>);
  static_assert(sincpp::is_string_v<std::string_view>);
  static_assert(sincpp::is_string_v<std::wstring_view>);
  static_assert(sincpp::is_string_v<char *>);
  static_assert(sincpp::is_string_v<char const *>);
  static_assert(sincpp::is_string_v<char[32]>);
}

TEST(is_char, not_string) {
  static_assert(sincpp::is_string_v<char> == false);
  static_assert(sincpp::is_string_v<int> == false);
  static_assert(sincpp::is_string_v<float> == false);
  static_assert(sincpp::is_string_v<double> == false);
  static_assert(sincpp::is_string_v<std::vector<char>> == false);
  static_assert(sincpp::is_string_v<std::array<char, 32>> == false);
}
