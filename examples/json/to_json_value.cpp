#include <sincpp/json/to_json_value.hpp>

#include <iostream>

int main() {
  std::cout << sincpp::to_json_value(3.14f).to_json() << std::endl; // 3.14
  std::cout << sincpp::to_json_value('c') << std::endl;             // c
  std::cout << sincpp::to_json_value("A string") << std::endl;      // A string

  return 0;
}
