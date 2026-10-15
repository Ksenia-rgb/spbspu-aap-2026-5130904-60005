#include <iostream>

namespace sequence_processing {
  bool increasing(long long int current, long long int next_val)
  {
    return next_val > current;
  }

  bool between(long long int prev, long long int curr, long long int next_val)
  {
    return (curr < prev) && (curr > next_val);
  }
}

int main()
{
  const int error_exit_code_1 = 1;
  const int error_exit_code_2 = 2;
  const int min_elements_for_grt_lss = 3;

  long long int a = 0;
  long long int b = 0;
  long long int c = 0;

  int current_length = 0;
  int max_length = 0;
  int total_elements = 0;
  int grt_lss_count = 0;

  if (!(std::cin >> a)) {
    std::cerr << "неверный формат данных" << std::endl;
    return error_exit_code_1;
  }

  if (a == 0) {
    std::cout << 0 << std::endl;
    std::cout << 0 << std::endl;
    return 0;
  }

  total_elements++;
  current_length = 1;
  max_length = 1;

  if (!(std::cin >> b)) {
    std::cerr << "неверный формат данных" << std::endl;
    return error_exit_code_1;
  }

  if (b == 0) {
    std::cout << max_length << std::endl;
    std::cerr << "мало элементов для второй характеристики" << std::endl;
    return error_exit_code_2;
  }

  total_elements++;
  if (sequence_processing::increasing(a, b)) {
    current_length++;
  } else {
    current_length = 1;
  }
  if (current_length > max_length) {
    max_length = current_length;
  }

  while (std::cin >> c) {
    if (c == 0) {
      break;
    }
    total_elements++;

    if (sequence_processing::increasing(b, c)) {
      current_length++;
    } else {
      current_length = 1;
    }

    if (current_length > max_length) {
      max_length = current_length;
    }

    if (sequence_processing::between(a, b, c)) {
      grt_lss_count++;
    }

    a = b;
    b = c;
  }

  if (!std::cin && c != 0) {
    std::cerr << "неккоретные символы в потоке" << std::endl;
    return error_exit_code_1;
  }

  std::cout << max_length << std::endl;

  if (total_elements < min_elements_for_grt_lss) {
    std::cerr << "мало элементов для второй характеристики" << std::endl;
    return error_exit_code_2;
  }

  std::cout << grt_lss_count << std::endl;
  return 0;
}
