#include <iostream>
#include <limits>

int main()
{
  const int input_error = 1;
  const int len_error = 2;
  const int good = 0;

  const int divisor = 2;
  long long num = 0, max_count = 0, current_count = 0;

  const long long max_len = std::numeric_limits< long long >::max();
  const int min_len = 2;
  long long curr_num = 0, len_of_seq = 0, prev_num = 0, count_of_del = 0;

  while (true) {
    std::cin >> num;
    if (std::cin.fail()) {
      std::cerr << "Ошибка ввода последовательности";
      return input_error;
    }
    if (num == 0) {
      break;
    }

    if (num % divisor == 0) {
      ++current_count;
      if (current_count > max_count) {
        max_count = current_count;
      }
    } else {
      current_count = 0;
    }

    curr_num = num;
    if (len_of_seq > 0 && curr_num % prev_num == 0) {
      ++count_of_del;
    }
    prev_num = curr_num;

    if (len_of_seq == max_len) {
      std::cerr << "Последовательность слишком большая";
      return len_error;
    }
    ++len_of_seq;
  }

  std::cout << "Характеристика №14: " << max_count << std::endl;

  if (len_of_seq < min_len) {
    std::cerr << "Последовательность слишком короткая";
    return len_error;
  }

  std::cout << "Характеристика №11: " << count_of_del << std::endl;
  return good;
}
