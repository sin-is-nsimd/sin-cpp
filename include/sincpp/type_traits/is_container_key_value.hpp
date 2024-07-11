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

#ifndef SINCPP_TYPE_TRAITS_IS_CONTAINER_KEY_VALUE_HPP
#define SINCPP_TYPE_TRAITS_IS_CONTAINER_KEY_VALUE_HPP

#include "../container/vector_pair.hpp"

#include <map>
#include <unordered_map>

namespace sincpp {

/**
 * @brief Type trait to check if a type is a key-value container type.
 *
 * `std::map`, `std::unordered_map` and sincpp::vector_pair_t are key-value
 * containers.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_container_key_value.cpp
 *
 * @ingroup sincpp_type_traits
 */
template <class T> struct is_container_key_value : std::false_type {};

/// @brief Template specialization for `std::map`.
/// @relates sincpp::is_container_key_value
template <class K, class T, class Compare, class Allocator>
struct is_container_key_value<std::map<K, T, Compare, Allocator>>
    : std::true_type {};

/// @brief Template specialization for `std::unordered_map`.
/// @relates sincpp::is_container_key_value
template <class K, class T, class Hash, class KeyEqual, class Allocator>
struct is_container_key_value<
    std::unordered_map<K, T, Hash, KeyEqual, Allocator>> : std::true_type {};

/// @brief Template specialization for vector_pair_t.
/// @relates sincpp::is_container_key_value
template <class K, class T>
struct is_container_key_value<vector_pair_t<K, T>> : std::true_type {};

/**
 * @brief Variable template to check if a type is a key-type container type.
 *
 * @tparam T The type to be checked.
 *
 * **Example**:
 * @include examples/type_traits/is_container_key_value.cpp
 *
 * @relates sincpp::is_container_key_value
 */
template <class T>
constexpr bool is_container_key_value_v = is_container_key_value<T>::value;

} // namespace sincpp

#endif
