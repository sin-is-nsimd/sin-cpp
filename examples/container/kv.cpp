#include <sincpp/container/kv.hpp>

#include <iostream>
#include <string_view>

int main() {
  sincpp::kv_t<char, std::string_view> const kv = {'K', "Value"};

  std::cout << "Key: " << kv.key << std::endl;
  std::cout << "Value: " << kv.val << std::endl;

  // Output:
  // Key: k
  // Value: Value

  return 0;
}
