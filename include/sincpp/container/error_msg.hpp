// Copyright © 2025 Lénaïc Bagnères, lenaicb@singularity.fr

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

#ifndef SINCPP_CONTAINER_ERROR_MSG_HPP
#define SINCPP_CONTAINER_ERROR_MSG_HPP

#include <string>

namespace sincpp {

/**
 * @brief Holds an error message, if it is not empty.
 *
 * Use sincpp::no_error to indicate the absence of an error.
 *
 * **Example:**
 * @include examples/container/error_msg.cpp
 *
 * @ingroup sincpp_container
 */
class error_msg_t : public std::string {
public:
  /// @brief Constructor from `std::string`.
  using std::string::string;

  /// @brief Constructor from a `std::string`.
  /// @param error_msg Error message.
  error_msg_t(std::string const &error_msg) : std::string{error_msg} {}

  /// @brief Conversion operator to `bool`.
  /// @returns true if there is an error message, false otherwise.
  explicit operator bool() const { return not this->empty(); }
};

/**
 * @brief Indicates there is no error message.
 *
 * @see sincpp::error_msg_t
 */
inline constexpr sincpp::error_msg_t no_error;

} // namespace sincpp

#endif
