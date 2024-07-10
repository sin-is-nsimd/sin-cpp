#include <sincpp/type_traits/is_container_key_value.hpp>

#include <iostream>
#include <vector>

int main() {
  std::cout << std::boolalpha;
  std::cout << sincpp::is_container_key_value_v<
                   std::unordered_map<std::string, int>> << std::endl; // true
  std::cout << sincpp::is_container_key_value_v<
                   std::vector<std::string>> << std::endl; // false

  return 0;
}
