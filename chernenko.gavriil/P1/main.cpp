#include <iostream>
#include <cstddef>

int main()
{
  const int exit_code_good = 0;
  const int exit_code_incorrect_input = 1;
  const int exit_code_short_seq = 2;
  const int stop_number = 0;
  const std::size_t min_seq_length = 2;
  std::size_t count = 0;
  std::size_t max_length = 0;
  std::size_t cur_length = 0;
  std::size_t i = 0;
  int prev_n = 0;
  while (true) {
    int n = 0;
    std::cin >> n;
    if (!std::cin) {
      std::cerr << "Incorrect input" << std::endl;
      return exit_code_incorrect_input;
    }
    if (n == stop_number) {
      std::cout << max_length << std::endl;
      if (i < min_seq_length) {
        std::cerr << "Too short sequence" << std::endl;
        return exit_code_short_seq;
      }
      break;
    }
    if (prev_n != 0) {
      if (n % prev_n == 0) {
        count++;
      }
      if (prev_n >= n) {
        cur_length += 1;
        if (cur_length > max_length) {
          max_length = cur_length;
        }
      }
    }
    prev_n = n;
    i++;
  }
  std::cout << count << std::endl;
  return exit_code_good;
}
