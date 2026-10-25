#include <iostream>
#include <cstddef>
namespace udalcova {

  const int error_input = 1;
  const int error_empty = 2;

  bool con(std::istream & cin, std::size_t & count, std::size_t & max, std::size_t & total)
  {
    max = 0;
    total = 0;
    count = 0;
    int a = 0;
    int b = 0;

    if (!(cin >> a)) {
      return false;
    }

    if (a == 0) {
      return true;
    }

    total = 1;
    bool has_prev = false;
    int next = 0;

    while (cin >> next && next != 0) {
      ++total;

      if (next > a) {
        ++count;
      }

      if (has_prev && a > b && a > next) {
        ++max;
      }

      b = a;
      a = next;
      has_prev = true;
    }

    if (cin.fail()) {
      return false;
    }
    return true;
  }
}

int main()
{
  std::size_t count = 0;
  std::size_t max = 0;
  std::size_t total = 0;

  if (!udalcova::con(std::cin, count, max, total)) {
    std::cerr << "Error: Ввод не является корректной последовательность\n";
    return udalcova::error_input;
  }
  std::cout << count << '\n';

  int result = 0;

  if (total == 0) {
    std::cerr << "Error: Последовательность пуста\n";
    result = udalcova::error_empty;
  } else {
    std::cout << max << '\n';
  }
  return result;
}
