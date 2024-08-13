#include <sincpp/json/to_json_value.hpp>

#include <iostream>

int main() {
  // int
  sincpp::number_str_t<sincpp::to_chars_max_size<int>()> const int_str =
      sincpp::make_number_str(42);
  std::cout << int_str.to_json() << std::endl; // 42

  // double
  sincpp::number_str_t<sincpp::to_chars_max_size<double>()> const double_str =
      sincpp::make_number_str(3.14);
  std::cout << double_str.to_json() << std::endl; // 3.14

  return 0;
}
