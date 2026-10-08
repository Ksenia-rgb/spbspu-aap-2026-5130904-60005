#include <iostream>
#include <algorithm>
#include <stdexcept>

int main()
{
  const int invalid_argument_error = 2;
  int res = 1;
  int k = 1;
  int k2 = 0;
  int a = 0;
  int b = 0;
  try {
    std::cin >> a;
    if (std::cin.fail()) {
      throw std::runtime_error("Input error");
    }
    if (a == 0) {
      std::cout << 0 << "\n";
      throw std::invalid_argument("Sequence is too short");
    }
    std::cin >> b;
    if (std::cin.fail()) {
      throw std::runtime_error("Input error");
    }
    if (b == 0) {
      std::cout << 1 << "\n";
      throw std::invalid_argument("Sequence is too short");
    }
    while (b != 0) {
      if (a == b) {
        k++;
      } else {
        res = std::max(k, res);
        k = 1;
      }
      if (b % a == 0) {
        k2++;
      }
      a = b;
      std::cin >> b;
      if (std::cin.fail()) {
        throw std::runtime_error("Input error");
      }
    }
    res = std::max(k, res);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << '\n';
    return invalid_argument_error;
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  std::cout << res << "\n" << k2 << "\n";
  return 0;
}
