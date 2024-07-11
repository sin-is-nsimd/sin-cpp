#include <sincpp/container/vector_pair.hpp>

#include <iostream>
#include <string>

int main() {
  sincpp::vector_pair_t<std::string, int> vp;
  vp["Zero"] = 0;
  vp["One"] = 1;
  vp["Two"] = 2;
  vp["Three"] = 3;

  std::cout << vp << std::endl; // { Zero: 0, One: 1, Two: 2, Three: 3 }

  return 0;
}
