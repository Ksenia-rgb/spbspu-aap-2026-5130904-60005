#include <iostream>
#include <limits>
int main()
{
  const int end = 0;
  const int input_error = 1;
  const int invalid_size = 2;
  int i = 0;
  try {
    int error_indicator = 0;
    int result = 0;
    int counter = 0;
    const int max_val = std::numeric_limits< int >::max();
    int previous = max_val;
    int a = 0;
    if (!(std::cin >> a)) {
      throw input_error;
    }
    if (a == 0) {
      std::cout << 0 << "\n" << 0 << "\n";
      return end;
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
      if (a <= previous) {
        if (counter == max_val) {
          error_indicator = 1;
          counter = 0;
        }
        counter++;
      }
      if (a > previous) {
        counter = 1;
      }
      previous = a;
      if (counter > result) {
        result = counter;
      }
      if (!(std::cin >> a)) {
        throw input_error;
      }
    }
    if (error_indicator == 1) {
      throw invalid_size;
    }
    std::cout << i << "\n";
    std::cout << result << "\n";
  } catch (int k) {
    if (k == input_error) {
      std::cerr << "Error" << "\n";
      return input_error;
    }
    if (k == invalid_size) {
      std::cout << i << "\n";
      std::cerr << "The lenght is incorrect";
      return invalid_size;
    }
  }
  return end;
}
