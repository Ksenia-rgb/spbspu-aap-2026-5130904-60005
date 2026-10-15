#include <cstddef>
#include <iostream>
#include <limits>

int main()
{
  const int code_invalid_input = 1;
  const int code_invalid_calculation = 2;
  const std::size_t required_elements = 2;
  const long long divisor_minus_one = -1;
  constexpr std::size_t max_count = std::numeric_limits< std::size_t >::max();

  long long prev = 0;
  long long first_max = std::numeric_limits< long long >::min();
  long long second_max = std::numeric_limits< long long >::min();
  std::size_t count = 0, seen_elements = 0;
  bool is_count_overflow = false;

  while (true) {
    long long curr = 0;
    std::cin >> curr;

    if (std::cin.fail()) {
      std::cerr << "Invalid argument\n";
      return code_invalid_input;
    }

    if (curr == 0) {
      break;
    }

    if (seen_elements == 0) {
      first_max = curr;
    } else {
      if (curr >= first_max) {
        second_max = first_max;
        first_max = curr;
      } else if (curr > second_max) {
        second_max = curr;
      }

      if (prev == divisor_minus_one || curr % prev == 0) {
        if (count == max_count) {
          is_count_overflow = true;
        } else {
          ++count;
        }
      }
    }

    prev = curr;

    if (seen_elements < required_elements) {
      ++seen_elements;
    }
  }

  if (seen_elements < required_elements) {
    std::cerr << "Too short\n";
    return code_invalid_calculation;
  }

  int result = 0;

  if (is_count_overflow) {
    std::cerr << "Count overflow\n";
    result = code_invalid_calculation;
  } else {
    std::cout << count << '\n';
  }

  std::cout << second_max << '\n';

  return result;
}
