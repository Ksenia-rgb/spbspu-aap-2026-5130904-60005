#include <iostream>

int main()
{
  int count = 1, count_pr = 0, count_pos = 0, cnt = 0;
  bool hasPr = false;
  while (count != 0) {
    std::cin >> count;
    if (std::cin.fail()) {
      std::cerr << "Invalid input\n";
      return 1;
    }
    if (hasPr) {
      count_pr = count_pos;
    }
    if (count != 0) {
      count_pos = count;
      hasPr = true;
    }
    if (count_pos != 0 && count_pr != 0) {
      if (count_pos > count_pr) {
        cnt++;
      }
    }
  }
  std::cout << cnt << "\n";
  return 0;
}
