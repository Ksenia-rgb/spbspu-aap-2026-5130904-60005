#include <iostream>

bool f(long long int a, long long int b, long long int c) {
  
  const long long int aq = a * a;
  const long long int bq = b * b;
  const long long int cq = c * c;

  if (!((aq + bq) == cq)) {
    return false;
  }
  return true;
}

int main() {
  const int min_required_args = 3;
  const int error_exit_code_2 = 2;

  long long int a = 0;
  long long int b = 0;
  long long int c = 0;
  int args = 0;
  int tri = 0;

  if (!(std::cin >> a)) {
    std::cerr << "Ошибка ввода" << std::endl;
    return 1;
  }
  if (a == 0) {
    std::cerr << "Мало значений" << std::endl;
    return error_exit_code_2;
  }
  args++;

  if (!(std::cin >> b)) {
    std::cerr << "Ошибка ввода" << std::endl;
    return 1;
  }
  if (b == 0) {
    std::cerr << "Мало значений" << std::endl;
    return error_exit_code_2;
  }
  args++;

  while (std::cin >> c) {
    if (c == 0) {
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
  if (args < min_required_args) {
    std::cerr << "Мало значений" << std::endl;
    return error_exit_code_2;
  }

  std::cout << args << std::endl;
  std::cout << tri << std::endl;
  return 0;
}
