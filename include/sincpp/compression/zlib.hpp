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

#ifndef SINCPP_COMPRESSION_GZIP_HPP
#define SINCPP_COMPRESSION_GZIP_HPP

#include "../container/result_code_message.hpp"

#include <cstddef>
#include <string_view>
#include <vector>

#include <zlib.h>

namespace sincpp::zlib {

/**
 * @brief Get the message corresponding to a `zlib` return code.
 *
 * @param zlib_return_code `zlib` return code.
 *
 * @returns the corresponding message.
 *
 * @ingroup sincpp_zlib
 */
std::string_view get_error_message(int const zlib_return_code) {
  switch (zlib_return_code) {
  case Z_OK:
    return "Z_OK";
  case Z_STREAM_END:
    return "Z_STREAM_END";
  case Z_NEED_DICT:
    return "Z_NEED_DICT";
  case Z_ERRNO:
    return "Z_ERRNO";
  case Z_STREAM_ERROR:
    return "Z_STREAM_ERROR";
  case Z_DATA_ERROR:
    return "Z_DATA_ERROR";
  case Z_MEM_ERROR:
    return "Z_MEM_ERROR";
  case Z_BUF_ERROR:
    return "Z_BUF_ERROR";
  case Z_VERSION_ERROR:
    return "Z_VERSION_ERROR";
  default:
    return "Unknown zlib return code";
  }
}

/**
 * @brief Compress data using `zlib`.
 *
 * This function uses `zlib` `deflate` function. If error occurs, output size
 * can be not 0.
 *
 * @tparam OutputContainer Output container.
 * @tparam InputContainer Input container.
 *
 * @param input Input data.
 *
 * @returns the compressed data, the final `zlib` return code and the error
 * message if something goes wrong.
 *
 * **Example:**
 * @include examples/compression/zlib.cpp
 *
 * @todo An overload function that takes an already allocated output.
 *
 * @ingroup sincpp_zlib
 */
template <class OutputContainer = std::vector<std::byte>, class InputContainer>
result_code_message_t<OutputContainer, int, std::string_view>
compress(InputContainer const &input) {
  // Output
  OutputContainer output;
  output.resize(compressBound(input.size()));

  // Init
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = input.size();
  stream.next_in =
      const_cast<Bytef *>(reinterpret_cast<Bytef const *>(input.data()));
  stream.avail_out = output.size();
  stream.next_out = reinterpret_cast<Bytef *>(output.data());
  {
    int const r = deflateInit(&stream, Z_BEST_COMPRESSION);
    if (r != Z_OK) {
      return {output, r, get_error_message(r)};
    }
  }

  // Compress & End
  int const r = deflate(&stream, Z_FINISH);
  deflateEnd(&stream);
  if (r != Z_STREAM_END) {
    return {output, r, get_error_message(r)};
  } else {
    output.resize(stream.total_out);
    return {output, Z_OK, ""};
  }
}

/**
 * @brief Decompress data using `zlib`.
 *
 * This function uses `zlib` `inflate` function. If error occurs, output size
 * can be not 0.
 *
 * @tparam OutputContainer Output container.
 * @tparam InputContainer Input container.
 *
 * @param input Input data (compressed).
 *
 * @returns the decompressed data, the final `zlib` return code and the error
 * message if something goes wrong.
 *
 * **Example:**
 * @include examples/compression/zlib.cpp
 *
 * @todo An overload function that takes an already allocated output.
 *
 * @ingroup sincpp_zlib
 */
template <class OutputContainer = std::vector<std::byte>, class InputContainer>
  requires(std::is_same_v<typename InputContainer::value_type, std::byte> &&
           sizeof(typename InputContainer::value_type) == sizeof(std::byte))
result_code_message_t<OutputContainer, int, std::string_view>
decompress(InputContainer const &input) {
  // Output
  OutputContainer output;
  output.resize(input.size() * 4);

  // Init
  z_stream stream;
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;
  stream.avail_in = input.size();
  stream.next_in =
      const_cast<Bytef *>(reinterpret_cast<Bytef const *>(input.data()));
  stream.avail_out = output.size();
  stream.next_out = reinterpret_cast<Bytef *>(output.data());
  {
    int const r = inflateInit(&stream);
    if (r != Z_OK) {
      return {output, r, get_error_message(r)};
    }
  }

  // Decompress & End
  while (true) {
    int const r = inflate(&stream, Z_NO_FLUSH);
    if (r == Z_OK) {
      continue;
    } else if (r == Z_STREAM_END) {
      inflateEnd(&stream);
      output.resize(stream.total_out);
      break;
    } else if (r == Z_BUF_ERROR) {
      output.resize(output.size() * 2);
      stream.avail_out = output.size() - stream.total_out;
      stream.next_out =
          reinterpret_cast<Bytef *>(output.data()) + stream.total_out;
      continue;
    } else {
      inflateEnd(&stream);
      return {output, r, get_error_message(r)};
    }
  }

  return {output, Z_OK, ""};
}

} // namespace sincpp::zlib

#endif
