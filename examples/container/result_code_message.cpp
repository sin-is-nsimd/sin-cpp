#include <sincpp/container/result_code_message.hpp>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string_view>

sincpp::result_code_message_t<int, bool, std::string_view> fct(int const i) {
  if (i < 0) {
    return {i, false, "Negative input"};
  }
  if (i == 0) {
    return {i, false, "Input is 0"};
  }
  return {i, true, ""};
}

int main() {
  srand(time(nullptr));
  int const input = rand() % 5 - 2;
  auto const &[i, code, message] = fct(input);

  if (code) {
    std::cout << "Result of fct(" << input << ") is " << i << std::endl;
  } else {
    std::cout << "Error with fct(" << input << "): " << message << std::endl;
  }

  // Possible outputs:
  // Error with fct(-2): Negative input
  // Error with fct(-1): Negative input
  // Error with fct(0): Input is 0
  // Result of fct(1) is 1
  // Result of fct(2) is 2

  return 0;
}
