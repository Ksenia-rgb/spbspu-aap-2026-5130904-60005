#include <cstddef>
#include <iostream>

int main()
{
  int n = 1, previous = 0, before_previous = 0;
  std::size_t count = 0, ans_for_15 = 0, ans_for_6 = 0, current_length_6 = 1;
  const int c1 = 1, c2 = 2, c3 = 3;
  const int exit_code_good = 0;
  const int exit_code_incorrect_input = 1;
  const int exit_code_short_seq = 2;
  while (true) {
    std::cin >> n;
    if (std::cin.fail()) {
      std::cerr << "Error!" << std::endl;
      return exit_code_incorrect_input;
    }
    if (n == 0) {
      break;
    }
    count += 1;
    if (count > c1) {
      if (n < previous) {
        current_length_6 = 1;
      } else {
        current_length_6 += 1;
      }
      if (current_length_6 > ans_for_6) {
        ans_for_6 = current_length_6;
      }
    }
    if (count > c2) {
      if (static_cast< long long >(n) == static_cast< long long >(previous) + before_previous) {
        ans_for_15 += 1;
      }
    }
    before_previous = previous;
    previous = n;
  }
  if (count == 1) {
    ans_for_6 = 1;
  }
  std::cout << ans_for_6 << std::endl;
  if (count < c3) {
    std::cerr << "Error!" << std::endl;
    return exit_code_short_seq;
  }
  std::cout << ans_for_15 << std::endl;
  return exit_code_good;
}
