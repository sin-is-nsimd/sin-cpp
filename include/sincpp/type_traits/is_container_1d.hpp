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

#ifndef SINCPP_TYPE_TRAITS_IS_CONTAINER_1D_HPP
#define SINCPP_TYPE_TRAITS_IS_CONTAINER_1D_HPP

#include <array>
#include <cstddef>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

namespace sincpp {

/**
 * @brief Type trait to check if a type is an 1D container type.
 *
 * `std::array`, `std::vector`, `std::list`, `std::forward_list`, `std::deque`,
 * `std::set` and `std::unordered_set` are 1D containers.
 *
 * `std::string` is not considered as an 1D container.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_container_1d.cpp
 *
 * @ingroup sincpp_type_traits
 */
template <class T> struct is_container_1d : std::false_type {};

/// @brief Template specialization for `std::array`.
/// @relates sincpp::is_container_1d
template <class T, size_t N>
struct is_container_1d<std::array<T, N>> : std::true_type {};

/// @brief Template specialization for `std::vector`.
/// @relates sincpp::is_container_1d
template <class T, class Allocator>
struct is_container_1d<std::vector<T, Allocator>> : std::true_type {};

/// @brief Template specialization for `std::list`.
/// @relates sincpp::is_container_1d
template <class T, class Allocator>
struct is_container_1d<std::list<T, Allocator>> : std::true_type {};

/// @brief Template specialization for `std::forward_list`.
/// @relates sincpp::is_container_1d
template <class T, class Allocator>
struct is_container_1d<std::forward_list<T, Allocator>> : std::true_type {};

/// @brief Template specialization for `std::deque`.
/// @relates sincpp::is_container_1d
template <class T, class Allocator>
struct is_container_1d<std::deque<T, Allocator>> : std::true_type {};

/// @brief Template specialization for `std::set`.
/// @relates sincpp::is_container_1d
template <class T, class Compare, class Allocator>
struct is_container_1d<std::set<T, Compare, Allocator>> : std::true_type {};

/// @brief Template specialization for `std::unordered_set`.
/// @relates sincpp::is_container_1d
template <class Key, class Hash, class KeyEqual, class Allocator>
struct is_container_1d<std::unordered_set<Key, Hash, KeyEqual, Allocator>>
    : std::true_type {};

/**
 * @brief Variable template to check if a type is an 1D container type.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_container_1d.cpp
 *
 * @relates sincpp::is_container_1d
 */
template <class T> constexpr bool is_container_1d_v = is_container_1d<T>::value;

} // namespace sincpp

#endif
