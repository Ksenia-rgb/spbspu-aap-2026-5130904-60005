#include <iostream>

bool f(long long int a, long long int b, long long int c)
{
  long long int aq = a * a;
  long long int bq = b * b;
  long long int cq = c * c;

  if (!((aq + bq) == cq)) {
    return false;
  }
  return true;
}

int main()
{
  long long int a, b, c;
  int args = 0;
  int tri = 0;

  if (!(std::cin >> a)) {
    std::cerr << "Ошибка ввода" << std::endl;
    return 1;
  }
  if (a == 0) {
    std::cerr << "Мало значений" << std::endl;
    return 2;
  }
  args++;

  if (!(std::cin >> b)) {
    std::cerr << "Ошибка ввода" << std::endl;
    return 1;
  }
  if (b == 0) {
    std::cerr << "Мало значений" << std::endl;
    return 2;
  }
  args++;

  while (std::cin >> c) {
    if (c == 0) {
      std::cout << "\n";
      break;
    }
    args++;

    if (f(a, b, c)) {
      tri += 1;
    }

    a = b;
    b = c;
  }

  if (!std::cin && c != 0) {
    std::cerr << "Ошибка ввода" << std::endl;
    return 1;
  }
  if (args < 3) {
    std::cerr << "Мало значений" << std::endl;
    return 2;
  }

  std::cout << args << std::endl;
  std::cout << tri << std::endl;
  return 0;
}
