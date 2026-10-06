#include <iostream>
#include <stdexcept>
#include <limits>
int func()
{
  const int err1 = 1;
  const int err2 = 2;
  const int err0 = 0;
  const int max_val = std::numeric_limits< int >::max();
  int a = 0;
  int count = 1;
  int i=0;
  std::cin >> a;
  if (std::cin.fail() && !std::cin.eof()) {
    throw err1;
  } else if (std::cin.eof()) {
    throw err2;
  }
  if (a == 0) {
    std::cout << 0;
    return err0;
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
    std::cin >> a;
    count++;
    if (count == max_val) {
      throw err2;
    }
    if (std::cin.fail()) {
      throw err1;
    }
  }
  return i;
}
int main()
{
  const int nole = 0;
  const int two = 2;
  const int one = 1;
  try {
    const int res = func();
    std::cout << res << "\n";
  } catch (int k) {
    if (k == one) {
      const int error1 = 1;
      std::cerr << "Error" << "\n";
      return error1;
    } else if (k == two) {
      const int error2 = 2;
      std::cerr << "The length is incorrect" << "\n";
      return error2;
    }
  }
  return nole;
}
