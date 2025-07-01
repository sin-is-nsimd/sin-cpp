#include <sincpp/container/error_msg.hpp>

#include <iostream>

auto check_input(int const input) //
    -> sincpp::error_msg_t {
  if (input <= 0) {
    return "Error: input value must be > 0";
  }
  return sincpp::no_error;
}

void fct(int const value) {
  sincpp::error_msg_t error_msg{check_input(value)};
  // Error
  if (error_msg) {
    std::cout << error_msg << std::endl;
  }
  // Ok
  else {
    std::cout << "Valid input: " << value << std::endl;
  }
}

int main() {
  fct(0); // Error: input value must be > 0
  fct(7); // Valid input: 7

  return 0;
}
