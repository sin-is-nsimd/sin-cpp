#include <sincpp/type_traits/is_char.hpp>

#include <iostream>

int main() {
  std::cout << std::boolalpha;
  std::cout << sincpp::is_char_v<char> << std::endl; // true
  std::cout << sincpp::is_char_v<int> << std::endl;  // false

  return 0;
}
