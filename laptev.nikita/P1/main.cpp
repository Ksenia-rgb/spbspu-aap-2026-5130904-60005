#include <iostream>

namespace laptev {

  bool equal(long long int prev, long long int curr)
  {
    return curr == prev;
  }

  bool increasing(long long int prev, long long int curr)
  {
    return curr >= prev;
  }

}

int main()
{
  const int error_exit_code_1 = 1;

  long long int a = 0;
  long long int b = 0;
  long long int c = 0;

  int eq_current = 0;
  int eq_max = 0;

  int inc_current = 0;
  int inc_max = 0;

  if (!(std::cin >> a)) {
    std::cerr << "invalid input" << std::endl;
    return error_exit_code_1;
  }

  if (a == 0) {
    std::cout << 0 << std::endl;
    std::cout << 0 << std::endl;
    return 0;
  }

  eq_current = 1;
  eq_max = 1;
  inc_current = 1;
  inc_max = 1;

  if (!(std::cin >> b)) {
    std::cerr << "invalid input" << std::endl;
    return error_exit_code_1;
  }

  if (b == 0) {
    std::cout << eq_max << std::endl;
    std::cout << inc_max << std::endl;
    return 0;
  }

  if (laptev::equal(a, b)) {
    eq_current++;
  } else {
    eq_current = 1;
  }
  if (eq_current > eq_max) {
    eq_max = eq_current;
  }

  if (laptev::increasing(a, b)) {
    inc_current++;
  } else {
    inc_current = 1;
  }
  if (inc_current > inc_max) {
    inc_max = inc_current;
  }

  while (std::cin >> c) {
    if (c == 0) {
      break;
    }

    if (laptev::equal(b, c)) {
      eq_current++;
    } else {
      eq_current = 1;
    }
    if (eq_current > eq_max) {
      eq_max = eq_current;
    }

    if (laptev::increasing(b, c)) {
      inc_current++;
    } else {
      inc_current = 1;
    }
    if (inc_current > inc_max) {
      inc_max = inc_current;
    }

    a = b;
    b = c;
  }

  if (!std::cin && c != 0) {
    std::cerr << "incorrect stream" << std::endl;
    return error_exit_code_1;
  }

  std::cout << eq_max << std::endl;
  std::cout << inc_max << std::endl;
  return 0;
}
