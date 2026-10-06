#include <iostream>

int main()
{
  const int divisor = 2;
  long long num = 0, max_count = 0, current_count = 0;
  while (true) {
    std::cin >> num;
    if (std::cin.fail()) {
      std::cerr << "Ошибка ввода";
      return 1;
    }
    if (num == 0) {
      break;
    }
    if (num % divisor == 0) {
      current_count++;
      if (current_count > max_count) {
        max_count = current_count;
      }
    } else {
      current_count = 0;
    }
  }

  std::cout << max_count << std::endl;

  return 0;
}
