#include <iostream>
#include <cstddef>

int main()
{
  const int exit_code_good = 0;
  const int exit_code_incorrect_input = 1;
  const int exit_code_short_seq = 2;
  const std::size_t min_seq_length = 2;
  std::size_t count = 0;
  std::size_t i = 0;
  int prev_n = 0;
  while (true) {
    int n = 0;
    std::cin >> n;
    if (!std::cin) {
      std::cerr << "Incorrect input" << std::endl;
      return exit_code_incorrect_input;
    }
    if (n == 0) {
      if (i < min_seq_length) {
        std::cerr << "Too short sequence" << std::endl;
        return exit_code_short_seq;
      }
      break;
    }
    if (prev_n != 0 && n % prev_n == 0) {
      count++;
    }
    prev_n = n;
    i++;
  }
  std::cout << count << std::endl;
  return exit_code_good;
}
