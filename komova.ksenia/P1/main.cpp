#include <iostream>
#include <stdexcept>

namespace komova {
  int max(int x, int y)
  {
    if (x > y) {
      return x;
    }
    return y;
  }
}

int main()
{
  const int invalid_argument_error = 2;
  const int runtime_error = 1;
  int res = 1;
  int count_povt = 1;
  int res2 = 0;
  int el1 = 0;
  int el2 = 0;
  try {
    if (not(std::cin >> el1)) {
      throw std::runtime_error("Input error");
    }
    if (el1 == 0) {
      std::cout << 0 << "\n";
      throw std::invalid_argument("Sequence is too short");
    }
    if (not(std::cin >> el2)) {
      throw std::runtime_error("Input error");
    }
    if (el2 == 0) {
      std::cout << 1 << "\n";
      throw std::invalid_argument("Sequence is too short");
    }
    while (el2 != 0) {
      if (el1 == el2) {
        count_povt++;
      } else {
        res = komova::max(count_povt, res);
        count_povt = 1;
      }
      if (el2 % el1 == 0) {
        res2++;
      }
      el1 = el2;
      if (not(std::cin >> el2)) {
        throw std::runtime_error("Input error");
      }
    }
    res = komova::max(count_povt, res);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << '\n';
    return invalid_argument_error;
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << '\n';
    return runtime_error;
  }
  std::cout << res << "\n" << res2 << "\n";
  return 0;
}
