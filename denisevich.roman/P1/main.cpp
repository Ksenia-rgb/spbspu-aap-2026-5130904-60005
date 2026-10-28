#include <iostream>
#include <limits>
int main()
{
  const int end = 0;
  const int input_error = 1;
  const int invalid_size = 2;
  int i = 0;
  int result = 0;
  bool error_indicator = 0;
  bool indicator_error_i = 0;
  try {
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
	indicator_error_i = false;
        i = 1;
      } else if (a == c) {
        if (i == max_val) {
          indicator_error_i = true;
          i = 0;
        }
        i++;
      }
      if (a <= previous) {
        if (counter == max_val) {
          error_indicator = true;
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
    if (error_indicator || indicator_error_i) {
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
      if (error_indicator && indicator_error_i) {
        std::cerr << "The length is incorrect" << "\n";
      } else if (error_indicator && !indicator_error_i) {
        std::cout << i << "\n";
        std::cerr << "Number 2 cant be calculated" << "\n";
      } else {
        std::cout << result << "\n";
        std::cerr << "Number 1 cant be calculated" << "\n";
      }
      return invalid_size;
    }
  }
  return end;
}
