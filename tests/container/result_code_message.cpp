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

#include <sincpp/container/result_code_message.hpp>

#include <string>
#include <string_view>

#include <gtest/gtest.h>

TEST(result_code_message, default_contructor) {
  sincpp::result_code_message_t<char, int, std::string_view> const r;
  EXPECT_EQ(r.result, char());
  EXPECT_EQ(r.code, 0);
  EXPECT_TRUE(r.message.empty());
}

TEST(result_code_message, contructor) {
  sincpp::result_code_message_t<std::string, bool, std::string_view> const r = {
      "Result", true, "Success"};
  EXPECT_EQ(r.result, "Result");
  EXPECT_EQ(r.code, true);
  EXPECT_EQ(r.message, "Success");
}
