#include <iostream>

int main()
{
  const int zero = 0;

  int count = 1;
  int count_pr = 0;
  int count_pos = 0;
  int cnt1 = 0;
  int cnt2 = 0;
  bool has_pr = false;

  while (count != zero) {
    std::cin >> count;
    if (std::cin.fail()) {
      std::cerr << "Invalid input\n";
      return 1;
    }

    if (has_pr) {
      count_pr = count_pos;
    }

    if (count != zero) {
      count_pos = count;
      has_pr = true;
    }

    if (count_pos != zero && count_pr != zero) {
      if (count_pos > count_pr) {
        cnt1++;
      }
      if ((count_pr < zero && count_pos > zero) || (count_pr > zero && count_pos < zero)) {
        cnt2++;
      }
    }
  }

  std::cout << cnt1 << "\n" << cnt2 << "\n";
  return 0;
}
