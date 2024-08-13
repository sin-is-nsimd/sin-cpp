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

#include <sincpp/json/to_json_value.hpp>

#include <gtest/gtest.h>

// numbers

TEST(to_json_value, int) {
  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const v = 42;
  EXPECT_EQ(v.to_json_size(), 2);
  EXPECT_EQ(v.to_json(), "42");
}

TEST(to_json_value, float) {
  sincpp::number_str_t<sincpp::to_chars_max_size<float>()> const v =
      sincpp::make_number_str(3.14f);
  EXPECT_EQ(v.to_json_size(), 4);
  EXPECT_EQ(v.to_json(), "3.14");
}

TEST(to_json_value, double) {
  auto const v = sincpp::make_number_str(21.42);
  static_assert(
      std::is_same_v<
          decltype(v),
          sincpp::number_str_t<sincpp::to_chars_max_size<double>()> const>);
  EXPECT_EQ(v.to_json_size(), 5);
  EXPECT_EQ(v.to_json(), "21.42");
}
