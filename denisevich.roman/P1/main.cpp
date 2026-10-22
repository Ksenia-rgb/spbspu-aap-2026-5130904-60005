#include <iostream>
#include <limits>
int main()
{
  const int end = 0;
  const int input_error = 1;
  const int noterror_justresult = 0;
  const int invalid_size = 2;
  try {
    int result = 0;
    int counter = 0;
    const int max_val = std::numeric_limits< int >::max();
    int previous = max_val;
    int a = 0;
    int count = 1;
    int i = 0;
    if (!(std::cin >> a)) {
      throw input_error;
    }
    if (a == 0) {
      std::cout << 0 << "\n" << 0 << "\n";
      return noterror_justresult;
    }
    int c = a;
    while (a != 0) {
      if (a < c) {
        c = a;
        i = 0;
        i++;
      } else if (a == c) {
        i++;
      }
      count++;
      if (count == max_val) {
        throw invalid_size;
      }
      if (a <= previous) {
        counter++;
      }
      if (a > previous) {
        counter = 1;
      }
      previous = a;
      if (counter > result) {
        result = counter;
      }
      std::cin >> a;
      if (std::cin.fail()) {
        throw input_error;
      }
    }
    std::cout << i << "\n";
    std::cout << result << "\n";
  } catch (int k) {
    if (k == input_error) {
      const int error1 = 1;
      std::cerr << "Error" << "\n";
      return error1;
    }
    if (k == invalid_size) {
      const int eror2 = 2;
      std::cerr << "The lenght is incorrect";
      return eror2;
    }
  }
  return end;
}
