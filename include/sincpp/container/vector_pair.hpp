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

#ifndef SINCPP_CONTAINER_VECTOR_PAIR_HPP
#define SINCPP_CONTAINER_VECTOR_PAIR_HPP

#include <iostream>
#include <ranges>
#include <utility>
#include <vector>

namespace sincpp {

/**
 * @brief Associative key-value pair container.
 *
 * This container contains key-value pairs with unique keys. The underlying
 * container is a `std::vector<std::pair<Key, T>>`.
 *
 * @tparam Key Key type.
 * @tparam T Mapped type.
 *
 * **Example:**
 * @include examples/container/vector_pair.cpp
 *
 * @ingroup sincpp_container
 */
template <class Key, class T> class vector_pair_t {
public:
  /// @brief Container type.
  using container_type = std::vector<std::pair<Key, T>>;

  /// @brief Key type.
  using key_type = Key;

  /// @brief Mapped type.
  using mapped_type = T;

  /// @brief Value type (e.g. `std::pair<Key, T>`).
  using value_type = typename container_type::value_type;

  /// @brief Size type.
  using size_type = typename container_type::size_type;

  /// @brief Difference type.
  using difference_type = typename container_type::difference_type;

  /// @brief Allocator type.
  using allocator_type = typename container_type::allocator_type;

  /// @brief Reference type.
  using reference = value_type &;

  /// @brief Const reference type.
  using const_reference = value_type const &;

  /// @brief Pointer type.
  using pointer = typename container_type::pointer;

  /// @brief Const pointer type.
  using const_pointer = typename container_type::const_pointer;

  /// @brief Iterator type.
  using iterator = typename container_type::iterator;

  /// @brief Const iterator type.
  using const_iterator = typename container_type::const_iterator;

  /// @brief Reverse iterator type.
  using reverse_iterator = typename container_type::reverse_iterator;

  /// @brief Const reverse iterator type.
  using const_reverse_iterator =
      typename container_type::const_reverse_iterator;

public:
  /// @brief Data.
  std::vector<std::pair<Key, T>> data;

public:
  /// @brief Default constructor.
  vector_pair_t() = default;

  /// @brief Constructor.
  /// @details This constructor is available for performance reason.
  /// @param size Size.
  /// @param default_value Default value.
  vector_pair_t(size_t const size, std::pair<Key, T> const &default_value = {})
      : data(size, default_value) {}

  // Element access

  /// @brief Finds the mapped value of the key.
  /// @details If the key does not exist, the element is inserted.
  /// @param key Key of the element.
  /// @return a reference to the mapped value.
  T &operator[](Key const &key) {
    auto it = find(key);
    if (it != end()) {
      return it->second;
    } else {
      return data.emplace_back(key, T{}).second;
    }
  }

  // Iterators

  /// @brief Iterator to the beginning.
  /// @returns an iterator to the beginning.
  const_iterator begin() const { return data.begin(); }

  /// @copydoc sincpp::vector_pair_t::begin
  iterator begin() { return data.begin(); }

  /// @copydoc sincpp::vector_pair_t::begin
  const_iterator cbegin() const { return data.begin(); }

  /// @brief Iterator to the end.
  /// @returns an iterator to the end.
  iterator end() { return data.end(); }

  /// @copydoc sincpp::vector_pair_t::end
  const_iterator end() const { return data.end(); }

  /// @copydoc sincpp::vector_pair_t::end
  const_iterator cend() const { return data.end(); }

  // Capacity

  /// @brief Checks if the container is empty.
  /// @returns true if the container is empty, false otherwise.
  bool empty() const { return data.empty(); }

  /// @brief Gets the number of elements.
  /// @returns the number of elements.
  size_t size() const { return data.size(); }

  /// @brief Gets the capacity.
  /// @returns the capacity.
  size_t capacity() const { return data.capacity(); }

  /// @brief Reserves storage.
  /// \param capacity New capacity.
  void reserve(size_t const capacity) { data.reserve(capacity); }

  // Modifiers

  /// @brief Constructs an element in-place.
  /// @details The element is inserted only if the key cannot be found. It the
  /// key is already present, the value is not updated.
  /// @param key Key of the element.
  /// @param value Value of the element.
  /// @returns a pair with an iterator to the element found or inserted and true
  /// if the element was inserted, false if the element was already present.
  template <class K, class V>
  std::pair<iterator, bool> emplace(K &&key, V &&value) {
    auto it = find(key);
    if (it != end()) {
      return {it, false};
    } else {
      data.emplace_back(std::forward<K>(key), std::forward<V>(value));
      return {data.end() - 1, true};
    }
  }

  /// @brief Removes an element.
  /// @param it Iterator to the element.
  /// @returns an iterator following the last removed element.
  iterator erase(iterator const it) { return data.erase(it); }

  /// @copydoc sincpp::vector_pair_t::erase
  const_iterator erase(const_iterator const it) { return data.erase(it); }

  /// @brief Removes an element.
  /// @param Key Key of the element.
  /// @returns the number of elements removed (1 or 0).
  size_t erase(Key const &key) {
    auto it = find(key);
    if (it != end()) {
      data.erase(it);
      return 1;
    } else {
      return 0;
    }
  }

  // Lookup

  /// @brief Finds an element.
  /// @param key Key of the element to search for.
  /// @returns an iterator to the found element, sincpp::vector_pair_t::end if
  /// the element is not found.
  const_iterator find(Key const &key) const {
    return std::ranges::find_if(data, [&](std::pair<Key, T> const &value) {
      return value.first == key;
    });
  }

  /// @copydoc sincpp::vector_pair_t::find
  iterator find(Key const &key) {
    return std::ranges::find_if(data, [&](std::pair<Key, T> const &value) {
      return value.first == key;
    });
  }
};

/**
 * @brief Operator `<<` between a `std::ostream` and a sincpp::vector_pair_t.
 *
 * @param o Output.
 * @param v A vector of pairs.
 *
 * @returns the `std::ostream`.
 *
 * \relates sincpp::vector_pair_t
 */
template <class Key, class T>
std::ostream &operator<<(std::ostream &o, vector_pair_t<Key, T> const &v) {
  o << "{";
  bool first = true;
  for (auto const &[key, value] : v) {
    if (first) {
      first = false;
    } else {
      o << ",";
    }
    o << " " << key << ": " << value;
  }
  o << " }";
  return o;
}

} // namespace sincpp

#endif
