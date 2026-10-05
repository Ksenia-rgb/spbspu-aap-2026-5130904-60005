#include <iostream>

int main()
{
  unsigned int exit_code = 0;
  size_t count = 0;
  size_t i = 0;
  int prev_n = 0;
  while (true) {
    int n = 0;
    std::cin >> n;
    if (!std::cin) {
      std::cerr << "Incorrect input" << std::endl;
      exit_code = 1;
      return exit_code;
    }
    if (n == 0) {
      if (i < 2) {
        std::cerr << "Too short sequence" << std::endl;
        exit_code = 2;
        return exit_code;
      }
      break;
    }
    if (prev_n != 0 && n % prev_n == 0) {
      count++;
    }
    prev_n = n;
    i++;
  }
  std::cout << count << std::endl;
  return exit_code;
}
