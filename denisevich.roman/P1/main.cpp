#include <iostream>
#include <limits>
int main()
{
  const int nole = 0;
  const int err1 = 1;
  const int err0 = 0;
  const int err2 = 2;
  try {
    int itog = 0;
    int schet = 0;
    const int max_val = std::numeric_limits< int >::max();
    int previous = max_val;
    int a = 0;
    int count = 1;
    int i = 0;
    if (!(std::cin >> a)) {
      throw err1;
    }
    if (a == 0) {
      std::cout << 0 << "\n" << 0 << "\n";
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
      count++;
      if (count == max_val) {
        throw err2;
      }
      if (a <= previous) {
        schet++;
      }
      if (a > previous) {
        schet = 1;
      }
      previous = a;
      if (schet > itog) {
        itog = schet;
      }
      std::cin >> a;
      if (std::cin.fail()) {
        throw err1;
      }
    }
    std::cout << i << "\n";
    std::cout << itog << "\n";
  } catch (int k) {
    if (k == err1) {
      const int error1 = 1;
      std::cerr << "Error" << "\n";
      return error1;
    }
    if (k == err2) {
      const int eror2 = 2;
      std::cerr << "The lenght is incorrect";
      return eror2;
    }
  }
  return nole;
}
