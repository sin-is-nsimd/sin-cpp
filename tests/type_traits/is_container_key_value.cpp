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

#include <sincpp/type_traits/is_container_key_value.hpp>

#include <gtest/gtest.h>

TEST(is_container_key_value, key_value_container) {
  static_assert( //
      sincpp::is_container_key_value_v<std::map<std::string, int>>);
  static_assert( //
      sincpp::is_container_key_value_v<std::unordered_map<int, std::string>>);
  static_assert( //
      sincpp::is_container_key_value_v<sincpp::vector_pair_t<char, float>>);
}

TEST(is_container_key_value, not_key_value_container) {
  static_assert(sincpp::is_container_key_value_v<int> == false);
  static_assert(sincpp::is_container_key_value_v<std::string> == false);
}
