#include <cstddef>
#include <iostream>

namespace demyanov {
  int run()
  {
    int a = 0;
    int a_prev = 0;
    int a_prev_2 = 0;
    std::size_t count = 0;

    std::size_t count_max = 0;
    std::size_t count_min = 0;

    while (true) {
      if (!(std::cin >> a)) {
        std::cerr << "Error: a isn't int" << "\n";
        return 1;
      }
      if (a == 0) {
        break;
      }

      if (count >= 2) {
        if (a_prev > a && a_prev > a_prev_2) {
          ++count_max;
        }
        if (a_prev < a && a_prev < a_prev_2) {
          ++count_min;
        }
      }

      ++count;

      a_prev_2 = a_prev;
      a_prev = a;
    }

    if (count == 0) {
      std::cerr << "Error: count = 0" << "\n";
      return 2;
    }

    std::cout << count_max << "\n";
    std::cout << count_min << "\n";
    return 0;
  }
}
int main()
{
  return demyanov::run();
}
