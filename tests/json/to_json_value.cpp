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

#include <sincpp/json/to_json_value.hpp>

#include <sstream>

#include <gtest/gtest.h>
#include <unordered_map>

// number

TEST(number_str, int) {
  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const a0 = 7;
  EXPECT_EQ(a0.to_json_size(), 1);
  EXPECT_EQ(a0.to_json(), "7");

  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const b0 = 42;
  EXPECT_EQ(b0.to_json_size(), 2);
  EXPECT_EQ(b0.to_json(), "42");

  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const c0 = 1138;
  EXPECT_EQ(c0.to_json_size(), 4);
  EXPECT_EQ(c0.to_json(), "1138");

  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const a1 = 7;
  EXPECT_EQ(a1.to_json_size(), 1);
  EXPECT_EQ(a1.to_json(), "7");

  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const b1 = 42;
  EXPECT_EQ(b1.to_json_size(), 2);
  EXPECT_EQ(b1.to_json(), "42");

  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const c1 = 1138;
  EXPECT_EQ(c1.to_json_size(), 4);
  EXPECT_EQ(c1.to_json(), "1138");

  EXPECT_EQ(a0, a0);
  EXPECT_EQ(a0, a1);
  EXPECT_EQ(a1, a0);
  EXPECT_EQ(a1, a1);
  EXPECT_EQ(b0, b0);
  EXPECT_EQ(b0, b1);
  EXPECT_EQ(b1, b0);
  EXPECT_EQ(b1, b1);
  EXPECT_EQ(c0, c0);
  EXPECT_EQ(c0, c1);
  EXPECT_EQ(c1, c0);
  EXPECT_EQ(c1, c1);

  EXPECT_NE(a0, b0);
  EXPECT_NE(a0, c0);
  EXPECT_NE(a0, b1);
  EXPECT_NE(a0, c1);
  EXPECT_NE(a1, b0);
  EXPECT_NE(a1, c0);
  EXPECT_NE(a1, b1);
  EXPECT_NE(a1, c1);

  EXPECT_NE(b0, a0);
  EXPECT_NE(b0, c0);
  EXPECT_NE(b0, a1);
  EXPECT_NE(b0, c1);
  EXPECT_NE(b1, a0);
  EXPECT_NE(b1, c0);
  EXPECT_NE(b1, a1);
  EXPECT_NE(b1, c1);

  EXPECT_NE(c0, a0);
  EXPECT_NE(c0, b0);
  EXPECT_NE(c0, a1);
  EXPECT_NE(c0, b1);
  EXPECT_NE(c1, a0);
  EXPECT_NE(c1, b0);
  EXPECT_NE(c1, a1);
  EXPECT_NE(c1, b1);
}

TEST(number_str, float) {
  sincpp::number_str_t<sincpp::to_chars_max_size<float>()> const v =
      sincpp::make_number_str(3.14f);
  EXPECT_EQ(v.to_json_size(), 4);
  EXPECT_EQ(v.to_json(), "3.14");
}

TEST(number_str, double) {
  auto const v = sincpp::make_number_str(21.42);
  static_assert(
      std::is_same_v<
          decltype(v),
          sincpp::number_str_t<sincpp::to_chars_max_size<double>()> const>);
  EXPECT_EQ(v.to_json_size(), 5);
  EXPECT_EQ(v.to_json(), "21.42");
}

TEST(to_json_value, number) {
  EXPECT_EQ(sincpp::to_json_value(42).to_json(), "42");
  EXPECT_EQ(sincpp::to_json_value(3.14f).to_json(), "3.14");
  EXPECT_EQ(sincpp::to_json_value(21.42).to_json(), "21.42");
}

TEST(to_json_value, ostream) {
  std::stringstream out;
  out << sincpp::to_json_value(42) << " "    //
      << sincpp::to_json_value(3.14f) << " " //
      << sincpp::to_json_value(21.42);
  EXPECT_EQ(out.str(), "42 3.14 21.42");
}

// string

TEST(to_json_value, char) { EXPECT_EQ(sincpp::to_json_value('c'), 'c'); }

TEST(to_json_value, string) {
  std::string const s = "A std::string";
  std::string const &v = sincpp::to_json_value(s);
  EXPECT_EQ(&v, &s);
}

TEST(to_json_value, string_view) {
  std::string_view const s = "A std::string_view";
  std::string_view const &v = sincpp::to_json_value(s);
  EXPECT_EQ(&v, &s);
}

TEST(to_json_value, char_const_ptr) {
  char const *s = "A char const *";
  char const *const &v = sincpp::to_json_value(s);
  EXPECT_EQ(&v, &s);
}

TEST(to_json_value, c_char_array) {
  char const c_array[] = "A C array";
  auto const &v = sincpp::to_json_value(c_array);
  static_assert(std::is_same_v<decltype(v), char const(&)[10]>);
  EXPECT_EQ(&v, &c_array);
}

// array, vector, list, deque, set, unordered_set

template <class C>
void test_container_number_check(C const &v,
                                 std::vector<std::string> const &expected) {
  EXPECT_EQ(v.size(), expected.size());
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i].to_json_size(), expected[i].size());
    EXPECT_EQ(v[i].to_json(), expected[i]);
  }
}

template <class C>
void test_container_number(C const &c,
                           std::vector<std::string> const &expected) {
  using T = typename C::value_type;
  std::vector<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>> const v =
      sincpp::to_json_value(c);
  test_container_number_check(v, expected);
}

template <class T, size_t N>
void test_container_number(std::array<T, N> const &c,
                           std::vector<std::string> const &expected) {
  std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N> const v =
      sincpp::to_json_value(c);
  test_container_number_check(v, expected);
}

template <class T, size_t N>
void test_container_number(std::array<std::string_view, N> const &c,
                           std::vector<std::string> const &expected) {
  std::array<sincpp::number_str_t<sincpp::to_chars_max_size<T>()>, N> const v =
      sincpp::to_json_value(c);
  test_container_number_check(v, expected);
}

template <class C> void test_container_string(C const &c) {
  C const &v = sincpp::to_json_value(c);
  EXPECT_EQ(&v, &c);
}

template <class T, size_t N>
void test_container_string(T const (&a)[N],
                           std::vector<std::string> const &expected) {
  std::array<std::string_view, N> const v = sincpp::to_json_value(a);
  EXPECT_EQ(v.size(), expected.size());
  for (size_t i = 0; i < v.size(); ++i) {
    EXPECT_EQ(v[i], expected[i]);
  }
}

TEST(to_json_value, container_number) {
  test_container_number(std::array<int, 0>(), {});
  test_container_number(std::array<int, 4>({7, 21, 42, 73}),
                        {"7", "21", "42", "73"});
  test_container_number(std::array<float, 2>({3.14f, 21.42f}),
                        {"3.14", "21.42"});
  test_container_number(std::array<double, 2>({3.14, 21.42}),
                        {"3.14", "21.42"});

  test_container_number(std::vector<int>(), {});
  test_container_number(std::vector<int>({7, 21, 42, 73}),
                        {"7", "21", "42", "73"});
  test_container_number(std::vector<float>({3.14f, 21.42f}), {"3.14", "21.42"});
  test_container_number(std::vector<double>({3.14, 21.42}), {"3.14", "21.42"});

  test_container_number(std::list<int>(), {});
  test_container_number(std::list<int>({7, 21, 42, 73}),
                        {"7", "21", "42", "73"});
  test_container_number(std::list<float>({3.14f, 21.42f}), {"3.14", "21.42"});
  test_container_number(std::list<double>({3.14, 21.42}), {"3.14", "21.42"});

  test_container_number(std::deque<int>(), {});
  test_container_number(std::deque<int>({7, 21, 42, 73}),
                        {"7", "21", "42", "73"});
  test_container_number(std::deque<float>({3.14f, 21.42f}), {"3.14", "21.42"});
  test_container_number(std::deque<double>({3.14, 21.42}), {"3.14", "21.42"});

  test_container_number(std::set<int>(), {});
  test_container_number(std::set<int>({7, 21, 42, 73}),
                        {"7", "21", "42", "73"});
  test_container_number(std::set<float>({3.14f, 21.42f}), {"3.14", "21.42"});
  test_container_number(std::set<double>({3.14, 21.42}), {"3.14", "21.42"});

  test_container_number(std::unordered_set<int>(), {});
  test_container_number(std::unordered_set<int>({73}), {"73"});
  test_container_number(std::unordered_set<float>({3.14f}), {"3.14"});
  test_container_number(std::unordered_set<double>({21.42}), {"21.42"});
}

TEST(to_json_value, container_string) {
  test_container_string(std::array<std::string, 0>());
  test_container_string(std::array<std::string_view, 0>());
  test_container_string(std::array<char const *, 0>());
  test_container_string(
      std::array<std::string, 4>({"A", "std::array", "of", "std::string"}));
  test_container_string(std::array<std::string_view, 4>(
      {"A", "std::array", "of", "std::string_view"}));
  test_container_string(
      std::array<char const *, 4>({"A", "std::array", "of", "char const *"}));

  {
    std::string const a[] = {"A", "C array", "of", "std::string"};
    test_container_string(a, {"A", "C array", "of", "std::string"});
  }
  {
    std::string const a[] = {"A", "C array", "of", "std::string_view"};
    test_container_string(a, {"A", "C array", "of", "std::string_view"});
  }
  {
    std::string const a[] = {"A", "C array", "of", "char const *"};
    test_container_string(a, {"A", "C array", "of", "char const *"});
  }

  test_container_string(std::vector<std::string>());
  test_container_string(std::vector<std::string_view>());
  test_container_string(std::vector<char const *>());
  test_container_string(
      std::vector<std::string>({"A", "std::vector", "of", "std::string"}));
  test_container_string(std::vector<std::string_view>(
      {"A", "std::vector", "of", "std::string_view"}));
  test_container_string(
      std::vector<char const *>({"A", "std::vector", "of", "char const *"}));

  test_container_string(std::list<std::string>());
  test_container_string(std::list<std::string_view>());
  test_container_string(std::list<char const *>());
  test_container_string(
      std::list<std::string>({"A", "std::list", "of", "std::string"}));
  test_container_string(std::list<std::string_view>(
      {"A", "std::list", "of", "std::string_view"}));
  test_container_string(
      std::list<char const *>({"A", "std::list", "of", "char const *"}));

  test_container_string(std::deque<std::string>());
  test_container_string(std::deque<std::string_view>());
  test_container_string(std::deque<char const *>());
  test_container_string(
      std::deque<std::string>({"A", "std::deque", "of", "std::string"}));
  test_container_string(std::deque<std::string_view>(
      {"A", "std::deque", "of", "std::string_view"}));
  test_container_string(
      std::deque<char const *>({"A", "std::deque", "of", "char const *"}));

  test_container_string(std::set<std::string>());
  test_container_string(std::set<std::string_view>());
  test_container_string(std::set<char const *>());
  test_container_string(
      std::set<std::string>({"A", "std::set", "of", "std::string"}));
  test_container_string(
      std::set<std::string_view>({"A", "std::set", "of", "std::string_view"}));
  test_container_string(
      std::set<char const *>({"A", "std::set", "of", "char const *"}));

  test_container_string(std::unordered_set<std::string>());
  test_container_string(std::unordered_set<std::string_view>());
  test_container_string(std::unordered_set<char const *>());
  test_container_string(std::unordered_set<std::string>(
      {"A", "std::unordered_set", "of", "std::string"}));
  test_container_string(std::unordered_set<std::string_view>(
      {"A", "std::unordered_set", "of", "std::string_view"}));
  test_container_string(std::unordered_set<char const *>(
      {"A", "std::unordered_set", "of", "char const *"}));
}

// map, unordered_map, sincpp::vector_pair_t

template <class C> void test_container_key_value_string_string(C const &c) {
  C const &r = sincpp::to_json_value(c);
  EXPECT_EQ(&r, &c);
}

TEST(to_json_value, container_key_value_string_string) {
  test_container_key_value_string_string<std::map<std::string, std::string>>(
      {{"1", "one"}, {"2", "two"}, {"3", "three"}});
  test_container_key_value_string_string<
      std::unordered_map<std::string_view, char const *>>(
      {{"1", "one"}, {"2", "two"}, {"3", "three"}});
  test_container_key_value_string_string<
      sincpp::vector_pair_t<std::string_view, std::string_view>>(
      {{"1", "one"}, {"2", "two"}, {"3", "three"}});
}

template <class C> void test_container_key_value_string_number(C const &c) {
  using T = typename C::mapped_type;
  sincpp::vector_pair_t<
      std::string_view,
      sincpp::number_str_t<sincpp::to_chars_max_size<T>()>> const r =
      sincpp::to_json_value(c);
  EXPECT_EQ(r.size(), c.size());
  for (auto const &[key, value] : c) {
    auto const it = r.find(key);
    EXPECT_NE(it, r.end());
    EXPECT_EQ(it->second, sincpp::to_json_value(value));
  }
}

TEST(to_json_value, container_key_value_string_number) {
  test_container_key_value_string_number<std::map<std::string, int>>(
      {{"1", 2}, {"2", 2}, {"3", 3}});
  test_container_key_value_string_number<
      std::unordered_map<std::string_view, float>>(
      {{"1", 1.f}, {"2", 2.f}, {"3", 3.f}});
  test_container_key_value_string_number<
      sincpp::vector_pair_t<std::string_view, double>>(
      {{"1", 1.f}, {"2", 2.f}, {"3", 3.0}});
}
