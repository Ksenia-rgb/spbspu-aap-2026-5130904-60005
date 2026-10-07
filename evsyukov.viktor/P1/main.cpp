#include <iostream>

int main()
{
  int count = 1, count_pr = 0, count_pos = 0, cnt1 = 0, cnt2 = 0;
  bool has_pr = false;
  while (count != 0) {
    std::cin >> count;
    if (std::cin.fail()) {
      std::cerr << "Invalid input\n";
      return 1;
    }
    if (has_pr) {
      count_pr = count_pos;
    }
    if (count != 0) {
      count_pos = count;
      has_pr = true;
    }
    if (count_pos != 0 && count_pr != 0) {
      if (count_pos > count_pr) {
        cnt1++;
      }
      if ((count_pr < 0 && count_pos > 0) || (count_pr > 0 && count_pos < 0)) {
        cnt2++;
      }
    }
  }
  std::cout << cnt1 << "\n" << cnt2 << "\n";
  return 0;
}
