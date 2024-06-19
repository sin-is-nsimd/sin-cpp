#include <sincpp/str/conv.hpp>

#include <iostream>

int main() {
  // int
  {
    std::array<char, sincpp::to_chars_max_size<int>()> r;
    auto const [r_ptr, errc] = sincpp::to_chars(r, 73);
    std::cout << std::string_view(r.data(), r_ptr) << std::endl; // 73
  }

  // double
  {
    std::array<char, sincpp::to_chars_max_size<double>()> r;
    auto const [r_ptr, errc] = sincpp::to_chars(r, 21.42);
    std::cout << std::string_view(r.data(), r_ptr) << std::endl; // 21.42
  }

  // error
  {
    std::array<char, 1> r;
    auto const [r_ptr, errc] = sincpp::to_chars(r, 1138);
    std::cout << std::make_error_code(errc).message()
              << std::endl; // Value too large for defined data type
  }

  return 0;
}
