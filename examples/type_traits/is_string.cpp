#include <sincpp/type_traits/is_string.hpp>

#include <iostream>

int main() {
  std::cout << std::boolalpha;
  std::cout << sincpp::is_string_v<std::string> << std::endl; // true
  std::cout << sincpp::is_string_v<int> << std::endl;         // false

  return 0;
}
