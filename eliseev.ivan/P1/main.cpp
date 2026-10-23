#include <iostream>
#include <limits>

int main()
{
  const long long smallch = std::numeric_limits< long long >::min();
  const int fail_code1 = 1;
  const int fail_code2 = 2;

  long long max1 = smallch;
  long long max2 = smallch;
  long long count = 0;
  long long count_max = 0;

  long long a = 0;
  while (std::cin >> a && a != 0) {
    ++count;

    if (a > max1) {
      max2 = max1;
      max1 = a;
      count_max = 1;
    } else if (a == max1) {
      ++count_max;
    } else if (a > max2) {
      max2 = a;
    }
  }

  if (!std::cin) {
    std::cerr << "ERROR: invalid input\n";
    return fail_code1;
  }

  if (count < 2 || max2 == smallch) {
    std::cerr << "ERROR: small posledovatelnost\n";
    return fail_code2;
  }

  std::cout << max2 << "\n";
  std::cout << count_max << "\n";

  return 0;
}
