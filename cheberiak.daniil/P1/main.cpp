#include <iostream>

int main() {
  int n = 1, count = 0, previous = 0, before_previous = 0, ans_for_15 = 0,
      ans_for_6 = 0, current_length_6 = 1;
  while (true) {
    std::cin >> n;
    if (std::cin.fail()) {
      std::cerr << "Error!" << std::endl;
      return 1;
    }
    if (n == 0) {
      break;
    }
    count += 1;
    if (count > 1) {
      if (n < previous) {
        current_length_6 = 1;
      } else {
        current_length_6 += 1;
      }
      if (current_length_6 > ans_for_6) {
        ans_for_6 = current_length_6;
      }
    }
    if (count > 2) {
      if (n == (previous + before_previous)) {
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
  if (count < 3) {
    std::cerr << "Error!" << std::endl;
    return 2;
  }
  std::cout << ans_for_15 << std::endl;
  return 0;
}
