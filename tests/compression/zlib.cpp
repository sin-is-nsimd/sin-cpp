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

#include <sincpp/compression/zlib.hpp>

#include <gtest/gtest.h>

template <class InputContainer>
std::vector<std::byte> compress(InputContainer const &input) {
  std::cout << "Data = " << std::endl;
  for (auto const e : input) {
    std::cout << e << " ";
  }
  std::cout << std::endl;

  auto const &[compressed, compressed_error_code, compressed_error_message] =
      sincpp::zlib::compress(input);

  if (compressed_error_message.empty() == false) {
    ADD_FAILURE() << "Compression error: " << compressed_error_message
                  << "(error code = " << compressed_error_code << ")";
  }

  std::cout << "Compressed data = ";
  for (std::byte const b : compressed) {
    std::cout << int(b) << " ";
  }
  std::cout << std::endl;

  return compressed;
}

template <class OutputContainer>
OutputContainer decompress(std::vector<std::byte> const &input) {
  auto const &[decompressed, decompressed_error_code,
               decompressed_error_message] =
      sincpp::zlib::decompress<OutputContainer>(input);

  if (decompressed_error_message.empty() == false) {
    ADD_FAILURE() << "Decompression error: " << decompressed_error_message
                  << "(error code = " << decompressed_error_code << ")";
  }

  return decompressed;
}

TEST(zlib, empty_string) {
  std::string_view const data;
  std::vector<std::byte> const r = compress(data);
  EXPECT_EQ(decompress<std::string>(r), data);
}

TEST(zlib, string) {
  std::string_view const data = "Bouh!";
  std::vector<std::byte> const r = compress(data);
  EXPECT_EQ(decompress<std::string>(r), data);
}

TEST(zlib, vector) {
  std::vector<int8_t> const data = {7, 21, 42, 73};
  std::vector<std::byte> const r = compress(data);
  EXPECT_EQ(decompress<std::vector<int8_t>>(r), data);
}

TEST(zlib, error) {
  auto const &[decompressed, decompressed_error_code,
               decompressed_error_message] =
      sincpp::zlib::decompress(std::vector<std::byte>());
  EXPECT_EQ(decompressed_error_code, Z_STREAM_ERROR);
  EXPECT_EQ(decompressed_error_message,
            sincpp::zlib::get_error_message(decompressed_error_code));
}

TEST(zlib, get_error_message) {
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_OK), "Z_OK");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_STREAM_END), "Z_STREAM_END");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_NEED_DICT), "Z_NEED_DICT");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_ERRNO), "Z_ERRNO");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_STREAM_ERROR), "Z_STREAM_ERROR");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_DATA_ERROR), "Z_DATA_ERROR");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_MEM_ERROR), "Z_MEM_ERROR");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_BUF_ERROR), "Z_BUF_ERROR");
  EXPECT_EQ(sincpp::zlib::get_error_message(Z_VERSION_ERROR),
            "Z_VERSION_ERROR");
  EXPECT_EQ(sincpp::zlib::get_error_message(42), "Unknown zlib return code");
}
