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

#include <sincpp/str/conv.hpp>

#include <gtest/gtest.h>

template <class T>
void test(T const num, std::string_view const str,
          std::vector<std::string_view> const &possible_results) {
  std::cout << "sizeof(T) = " << sizeof(T) << " | num = " << num
            << " | str_size = " << str.size() << " | str = " << str
            << std::endl;

  bool ok = false;
  std::string possible_results_str;
  for (std::string_view const expected : possible_results) {
    possible_results_str += " \"" + std::string(expected) + "\"";
    if (str == expected) {
      ok = true;
      EXPECT_EQ(str.size(), expected.size());
      EXPECT_EQ(str, expected);
      break;
    }
  }

  if (ok == false) {
    ADD_FAILURE() << "Result \"" << str << "\" not in" << possible_results_str;
  }
}

template <class T>
void test_int(T const value,
              std::vector<std::string_view> const &possible_results) {
  std::array<char, sincpp::to_chars_max_size<T>()> r;
  auto const [r_ptr, errc] = sincpp::to_chars(r, value);
  test(value, std::string_view(r.data(), r_ptr), possible_results);
}

template <std::chars_format FloatFormat = std::chars_format::general, class T>
void test_float(T const value,
                std::vector<std::string_view> const &possible_results) {
  std::array<char, sincpp::to_chars_max_size<T, FloatFormat>()> r;
  auto const [r_ptr, errc] = sincpp::to_chars<FloatFormat>(r, value);
  test(value, std::string_view(r.data(), r_ptr), possible_results);
}

template <class T>
void test_scientific(T const value,
                     std::vector<std::string_view> const &possible_results) {
  test_float<std::chars_format::scientific>(value, possible_results);
}

template <class T>
void test_fixed(T const value,
                std::vector<std::string_view> const &possible_results) {
  test_float<std::chars_format::fixed>(value, possible_results);
}

template <class T>
void test_hex(T const value,
              std::vector<std::string_view> const &possible_results) {
  test_float<std::chars_format::hex>(value, possible_results);
}

TEST(to_chars, int) {
  test_int(0, {"0"});
  test_int(-0, {"0"});

  test_int(1, {"1"});
  test_int(-1, {"-1"});

  test_int(42, {"42"});
  test_int(1138, {"1138"});
  test_int(-8000, {"-8000"});

  test_int(std::numeric_limits<int8_t>::max(), {"127"});
  test_int(std::numeric_limits<int8_t>::min(), {"-128"});

  test_int(std::numeric_limits<int16_t>::max(), {"32767"});
  test_int(std::numeric_limits<int16_t>::min(), {"-32768"});

  test_int(std::numeric_limits<int32_t>::max(), {"2147483647"});
  test_int(std::numeric_limits<int32_t>::min(), {"-2147483648"});

  test_int(std::numeric_limits<int64_t>::max(), {"9223372036854775807"});
  test_int(std::numeric_limits<int64_t>::min(), {"-9223372036854775808"});

  test_int(std::numeric_limits<uint8_t>::max(), {"255"});
  test_int(std::numeric_limits<uint8_t>::min(), {"0"});

  test_int(std::numeric_limits<uint16_t>::max(), {"65535"});
  test_int(std::numeric_limits<uint16_t>::min(), {"0"});

  test_int(std::numeric_limits<uint32_t>::max(), {"4294967295"});
  test_int(std::numeric_limits<uint32_t>::min(), {"0"});

  test_int(std::numeric_limits<uint64_t>::max(), {"18446744073709551615"});
  test_int(std::numeric_limits<uint64_t>::min(), {"0"});
}

TEST(to_chars, float) {
  test_float(0.f, {"0"});
  test_float(21.42f, {"21.42"});
  test_float(-0.4f, {"-0.4"});
  test_float(std::numeric_limits<float>::max(), {"3.4028235e+38"});
  test_float(std::numeric_limits<float>::min(), {"1.1754944e-38"});
  test_float(-std::numeric_limits<float>::min(), {"-1.1754944e-38"});
  test_float(std::numeric_limits<float>::lowest(), {"-3.4028235e+38"});

  test_float(0.0, {"0"});
  test_float(21.42, {"21.42"});
  test_float(-0.4, {"-0.4"});
  test_float(std::numeric_limits<double>::max(), {"1.7976931348623157e+308"});
  test_float(std::numeric_limits<double>::min(), {"2.2250738585072014e-308"});
  test_float(-std::numeric_limits<double>::min(), {"-2.2250738585072014e-308"});
  test_float(std::numeric_limits<double>::lowest(),
             {"-1.7976931348623157e+308"});

  test_float(std::numeric_limits<long double>::max(),
             {
                 "1.189731495357231765e+4932", // clang
                 "Infinity",                   // gcc
                 "1.7976931348623157e+308"     // armhf
             });
  test_float(std::numeric_limits<long double>::min(),
             {
                 "3.3621031431120935063e-4932", // clang
                 "0",                           // gcc
                 "2.2250738585072014e-308"      // armhf
             });
  test_float(-std::numeric_limits<long double>::min(),
             {
                 "-3.3621031431120935063e-4932", // clang
                 "-0",                           // gcc
                 "-2.2250738585072014e-308"      // armhf
             });
  test_float(std::numeric_limits<long double>::lowest(),
             {
                 "-1.189731495357231765e+4932", // clang
                 "-Infinity",                   // gcc
                 "-1.7976931348623157e+308"     // armhf
             });
}

TEST(to_chars, float_scientific) {
  test_scientific(0.f, {"0e+00"});
  test_scientific(21.42f, {"2.142e+01"});
  test_scientific(-0.4f, {"-4e-01"});
  test_scientific(std::numeric_limits<float>::max(), {"3.4028235e+38"});
  test_scientific(std::numeric_limits<float>::min(), {"1.1754944e-38"});
  test_scientific(-std::numeric_limits<float>::min(), {"-1.1754944e-38"});
  test_scientific(std::numeric_limits<float>::lowest(), {"-3.4028235e+38"});

  test_scientific(0.0, {"0e+00"});
  test_scientific(21.42, {"2.142e+01"});
  test_scientific(-0.4, {"-4e-01"});
  test_scientific(std::numeric_limits<double>::max(),
                  {"1.7976931348623157e+308"});
  test_scientific(std::numeric_limits<double>::min(),
                  {"2.2250738585072014e-308"});
  test_scientific(-std::numeric_limits<double>::min(),
                  {"-2.2250738585072014e-308"});
  test_scientific(std::numeric_limits<double>::lowest(),
                  {"-1.7976931348623157e+308"});

  test_scientific(std::numeric_limits<long double>::max(),
                  {
                      "1.189731495357231765e+4932", // clang
                      "Infinity",                   // gcc
                      "1.7976931348623157e+308"     // armhf
                  });
  test_scientific(std::numeric_limits<long double>::min(),
                  {
                      "3.3621031431120935063e-4932", // clang
                      "0e+00",                       // gcc
                      "2.2250738585072014e-308"      // armhf
                  });
  test_scientific(-std::numeric_limits<long double>::min(),
                  {
                      "-3.3621031431120935063e-4932", // clang
                      "-0e+00",                       // gcc
                      "-2.2250738585072014e-308"      // armhf
                  });
  test_scientific(std::numeric_limits<long double>::lowest(),
                  {
                      "-1.189731495357231765e+4932", // clang
                      "-Infinity",                   // gcc
                      "-1.7976931348623157e+308"     // armhf
                  });
}

TEST(to_chars, float_fixed) {
  test_fixed(0.f, {"0"});
  test_fixed(21.42f, {"21.42"});
  test_fixed(-0.4f, {"-0.4"});
  test_fixed(std::numeric_limits<float>::max(),
             {"340282346638528859811704183484516925440"});
  test_fixed(std::numeric_limits<float>::min(),
             {"0.000000000000000000000000000000000000011754944"});
  test_fixed(-std::numeric_limits<float>::min(),
             {"-0.000000000000000000000000000000000000011754944"});
  test_fixed(std::numeric_limits<float>::lowest(),
             {"-340282346638528859811704183484516925440"});

  test_fixed(0.0, {"0"});
  test_fixed(21.42, {"21.42"});
  test_fixed(-0.4, {"-0.4"});
  test_fixed(
      std::numeric_limits<double>::max(),
      {"17976931348623157081452742373170435679807056752584499659891747680"
       "31572607800285387605895586327668781715404589535143824642343213268"
       "89464182768467546703537516986049910576551282076245490090389328944"
       "07586850845513394230458323690322294816580855933212334827479782620"
       "4144723168738177180919299881250404026184124858368"});
  test_fixed(std::numeric_limits<double>::min(),
             {"0."
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "000000000000000000022250738585072014"});
  test_fixed(-std::numeric_limits<double>::min(),
             {"-0."
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "00000000"
              "000000000000000000022250738585072014"});
  test_fixed(
      std::numeric_limits<double>::lowest(),
      {"-1797693134862315708145274237317043567980705675258449965989174768"
       "03157260780028538760589558632766878171540458953514382464234321326"
       "88946418276846754670353751698604991057655128207624549009038932894"
       "40758685084551339423045832369032229481658085593321233482747978262"
       "04144723168738177180919299881250404026184124858368"});

  // long double not supported
}

TEST(to_chars, float_hex) {
  test_hex(0.f, {"0p+0"});
  test_hex(21.42f, {"1.56b852p+4"});
  test_hex(-0.4f, {"-1.99999ap-2"});
  test_hex(std::numeric_limits<float>::max(), {"1.fffffep+127"});
  test_hex(std::numeric_limits<float>::min(), {"1p-126"});
  test_hex(-std::numeric_limits<float>::min(), {"-1p-126"});
  test_hex(std::numeric_limits<float>::lowest(), {"-1.fffffep+127"});

  test_hex(0.0, {"0p+0"});
  test_hex(21.42, {"1.56b851eb851ecp+4"});
  test_hex(-0.4, {"-1.999999999999ap-2"});
  test_hex(std::numeric_limits<double>::max(), {"1.fffffffffffffp+1023"});
  test_hex(std::numeric_limits<double>::min(), {"1p-1022"});
  test_hex(-std::numeric_limits<double>::min(), {"-1p-1022"});
  test_hex(std::numeric_limits<double>::lowest(), {"-1.fffffffffffffp+1023"});

  test_hex(static_cast<long double>(0.0), {
                                              "0p+0",    // clang
                                              "0p-16385" // gcc
                                          });
  test_hex(static_cast<long double>(21.42), {
                                                "a.b5c28f5c28f6p+1", // amd64
                                                "1.56b851eb851ecp+4" // arm
                                            });
  test_hex(static_cast<long double>(-0.4), {
                                               "-c.cccccccccccdp-5", // amd64
                                               "-1.999999999999ap-2" // arm
                                           });
  test_hex(std::numeric_limits<long double>::max(),
           {
               "f.fffffffffffffffp+16380", // amd64, clang
               "8p+16381",                 // amd64, gcc
               "1.fffffffffffffp+1023"     // arm
           });
  test_hex(std::numeric_limits<long double>::min(),
           {
               "8p-16385", // amd64, clang
               "0p-16385", // amd64, gcc
               "1p-1022"   // arm
           });
  test_hex(-std::numeric_limits<long double>::min(),
           {
               "-8p-16385", // amd64, clang
               "-0p-16385", // amd64, gcc
               "-1p-1022"   // arm
           });
  test_hex(std::numeric_limits<long double>::lowest(),
           {
               "-f.fffffffffffffffp+16380", // amd64, clang
               "-8p+16381",                 // amd64, gcc
               "-1.fffffffffffffp+1023"     // arm
           });
}
