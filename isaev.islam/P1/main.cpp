#include <iostream>
#include <limits>

namespace isaev {
  bool increasing(long long int current, long long int next_val)
  {
    return next_val >= current;
  }

  bool between(long long int prev, long long int curr, long long int next_val)
  {
    return (curr < prev) && (curr > next_val);
  }
}

int main()
{
  const int error_exit_code = 1;
  const int error_exit_code_overflow = 2;
  const int max_int = std::numeric_limits< int >::max();

  long long int a = 0;
  long long int b = 0;
  long long int c = 0;

  int current_length = 0;
  int max_length = 0;
  int grt_lss_count = 0;
  bool zero_found = false;
  bool length_overflow = false;
  bool count_overflow = false;

  if (!(std::cin >> a)) {
    std::cerr << "uncorrect data format" << std::endl;
    return error_exit_code;
  }

  if (a == 0) {
    std::cout << 0 << std::endl;
    std::cout << 0 << std::endl;
    return 0;
  }
  current_length = 1;
  max_length = 1;

  if (!(std::cin >> b)) {
    std::cerr << "uncorrect data format" << std::endl;
    return error_exit_code;
  }

  if (b == 0) {
    std::cout << max_length << std::endl;
    std::cout << 0 << std::endl;
    return 0;
  }

  if (isaev::increasing(a, b)) {
    current_length++;
  } else {
    current_length = 1;
  }
  if (current_length > max_length) {
    max_length = current_length;
  }

  while (std::cin >> c) {
    if (c == 0) {
      zero_found = true;
      break;
    }

    if (isaev::increasing(b, c)) {
      if (current_length == max_int) {
        length_overflow = true;
      } else {
        current_length++;
      }
    } else {
      current_length = 1;
    }

    if (current_length > max_length) {
      max_length = current_length;
    }

    if (isaev::between(a, b, c)) {
      if (grt_lss_count == max_int) {
        count_overflow = true;
      } else {
        grt_lss_count++;
      }
    }

    a = b;
    b = c;
  }

  if (!zero_found) {
    std::cerr << "uncorrect symbols" << std::endl;
    return error_exit_code;
  }

  if (length_overflow) {
    std::cerr << "overflow" << std::endl;
  } else {
    std::cout << max_length << std::endl;
  }
  if (count_overflow) {
    std::cerr << "overflow" << std::endl;
  } else {
    std::cout << grt_lss_count << std::endl;
  }
  if (length_overflow || count_overflow) {
    return error_exit_code_overflow;
  }
  return 0;
}
