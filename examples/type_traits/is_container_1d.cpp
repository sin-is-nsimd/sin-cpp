#include <sincpp/type_traits/is_container_1d.hpp>

#include <iostream>

int main() {
  std::cout << std::boolalpha;
  std::cout << sincpp::is_container_1d_v<std::vector<int>> << std::endl; // true
  std::cout << sincpp::is_container_1d_v<std::string> << std::endl; // false

  return 0;
}
