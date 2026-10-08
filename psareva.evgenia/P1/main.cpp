#include <iostream>
#include <limits>

int main()
{
  const int good = 0;
  const int incorrect_input = 1;
  const int short_sequence = 2;
  int number = 0;
  int maxx = std::numeric_limits< int >::min();
  int prev_number = std::numeric_limits< int >::max();
  size_t count_zd3 = 0;
  size_t length_zd5 = 0;
  size_t length_zd5_max = 0;
  std::cin >> number;
  if (std::cin.fail()) {
    std::cerr << "Incorrect input" << std::endl;
    return incorrect_input;
  }
  while (number != 0) {
    if (number > maxx) {
      maxx = number;
      count_zd3 = 0;
    }
    if (number == maxx) {
      count_zd3++;
    }
    if (number <= prev_number) {
      length_zd5++;
    }
    if (number > prev_number) {
      length_zd5 = 0;
    }
    prev_number = number;
    if (length_zd5 > length_zd5_max) {
      length_zd5_max = length_zd5;
    }
    std::cin >> number;
    if (std::cin.fail()) {
      std::cerr << "Incorrect input" << std::endl;
      return incorrect_input;
    }
  }
  if (count_zd3 == 0) {
    std::cerr << "Too short sequence" << std::endl;
    std::cout << length_zd5_max << std::endl;
    return short_sequence;
  }
  std::cout << count_zd3 << std::endl;
  std::cout << length_zd5_max << std::endl;
  return good;
}
