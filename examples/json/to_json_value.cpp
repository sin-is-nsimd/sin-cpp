#include <sincpp/json/to_json_value.hpp>

#include <iostream>

template <class C> void print(C const &c) {
  for (auto const &e : c) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::cout << sincpp::to_json_value(3.14f).to_json() << std::endl; // 3.14
  std::cout << sincpp::to_json_value('c') << std::endl;             // c
  std::cout << sincpp::to_json_value("A string") << std::endl;      // A string

  int const c_array_i[] = {1, 2, 3};
  char const c_array_s[] = {'a', 'b', 'c'};
  std::array<int, 3> const array_i = {21, 42, 73};
  std::array<std::string, 2> const array_s = {"An", "array"};
  print(sincpp::to_json_value(c_array_i)); // 1 2 3
  print(sincpp::to_json_value(c_array_s)); // a b c
  print(sincpp::to_json_value(array_i));   // 21 42 73
  print(sincpp::to_json_value(array_s));   // An array

  std::vector<char const *> const vector = {"A", "std::vector"};
  std::list<std::string_view> const list = {"A", "std::list"};
  std::set<std::string> const set = {"A", "std::set"};
  print(sincpp::to_json_value(vector)); // A std::vector
  print(sincpp::to_json_value(list));   // A std::list
  print(sincpp::to_json_value(set));    // A std::set

  return 0;
}
