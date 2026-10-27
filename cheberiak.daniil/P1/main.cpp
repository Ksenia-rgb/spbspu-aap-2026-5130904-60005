#include <cstddef>
#include <iostream>

int main()
{
  int n = 1, previous = 0, before_previous = 0;
  std::size_t count = 0, ans_for_15 = 0, ans_for_6 = 0, current_length_6 = 1;
  const std::size_t first_element = 1;
  const std::size_t second_element = 2;
  const std::size_t min_sequence_length = 3;
  const int exit_code_good = 0;
  const int exit_code_incorrect_input = 1;
  const int exit_code_short_seq = 2;
  while (std::cin >> n) {
    if (n == 0) {
      break;
    }
    count += 1;
    if (count > first_element) {
      if (n < previous) {
        current_length_6 = 1;
      } else {
        current_length_6 += 1;
      }
      if (current_length_6 > ans_for_6) {
        ans_for_6 = current_length_6;
      }
    }
    if (count > second_element) {
      if (static_cast< long long >(n) == static_cast< long long >(previous) + before_previous) {
        ans_for_15 += 1;
      }
    }
    before_previous = previous;
    previous = n;
  }
  if (std::cin.fail()) {
    std::cerr << "Error!" << std::endl;
    return exit_code_incorrect_input;
  }
  if (count == 1) {
    ans_for_6 = 1;
  }
  std::cout << ans_for_6 << std::endl;
  if (count < min_sequence_length) {
    std::cerr << "Error!" << std::endl;
    return exit_code_short_seq;
  }
  std::cout << ans_for_15 << std::endl;
  return exit_code_good;
}
