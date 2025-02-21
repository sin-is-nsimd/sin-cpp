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

#include <sincpp/container/vector_pair.hpp>

#include <gtest/gtest.h>

TEST(vector_pair, default_contructor) {
  sincpp::vector_pair_t<std::string, int> vp;
  EXPECT_EQ(vp.size(), 0);
  EXPECT_TRUE(vp.empty());
}

TEST(vector_pair, contructor) {
  sincpp::vector_pair_t<std::string, int> vp{7};
  EXPECT_EQ(vp.size(), 7);
  EXPECT_FALSE(vp.empty());
}

TEST(vector_pair, reserve) {
  sincpp::vector_pair_t<std::string, int> vp;
  vp.reserve(4096);
  EXPECT_EQ(vp.size(), 0);
  EXPECT_TRUE(vp.empty());
  EXPECT_EQ(vp.capacity(), 4096);
}

TEST(vector_pair, vector_pair_init_list) {
  sincpp::vector_pair_t<int, std::string> vp{
      {0, "Zero"}, {1, "One"}, {2, "Two"}};
  EXPECT_EQ(vp[0], "Zero");
  EXPECT_EQ(vp[1], "One");
  EXPECT_EQ(vp[2], "Two");
}

TEST(vector_pair, vector_pair) {
  sincpp::vector_pair_t<std::string, int> vp;

  vp["Zero"] = 0;
  vp["One"] = 1;
  vp.emplace("Three", 3);

  {
    int i = 0;
    for (auto it = vp.begin(); it != vp.end(); ++it) {
      EXPECT_EQ(&*it, &*(vp.begin() + i));
      ++i;
    }
  }

  {
    int i = 0;
    for (auto it = vp.cbegin(); it != vp.cend(); ++it) {
      EXPECT_EQ(&*it, &*(vp.begin() + i));
      ++i;
    }
  }

  EXPECT_EQ(vp.size(), 3);
  EXPECT_FALSE(vp.empty());
  EXPECT_EQ(vp["Zero"], 0);
  EXPECT_EQ(vp["One"], 1);
  EXPECT_EQ(vp["Three"], 3);

  vp.erase("Three");

  EXPECT_EQ(vp.size(), 2);
  EXPECT_FALSE(vp.empty());
  EXPECT_EQ(vp["Zero"], 0);
  EXPECT_EQ(vp["One"], 1);

  vp.emplace("Two", 2);

  EXPECT_EQ(vp.size(), 3);
  EXPECT_FALSE(vp.empty());
  EXPECT_EQ(vp["Zero"], 0);
  EXPECT_EQ(vp["One"], 1);
  EXPECT_EQ(vp["Two"], 2);

  vp.erase(vp.find("Two"));

  EXPECT_EQ(vp.size(), 2);
  EXPECT_FALSE(vp.empty());
  EXPECT_EQ(vp["Zero"], 0);
  EXPECT_EQ(vp["One"], 1);

  sincpp::vector_pair_t<std::string, int> const &cref = vp;
  vp.erase(cref.find("One"));

  vp.erase(vp.find("Zero"));
  EXPECT_EQ(vp.size(), 0);
  EXPECT_TRUE(vp.empty());
}
