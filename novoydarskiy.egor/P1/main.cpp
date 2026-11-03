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

  bool max_count_overflow = false;
  bool count_of_del_overflow = false;

  while (std::cin >> num) {
    if (num == 0) {
      break;
    }

    if (num % divisor == 0) {
      if (!max_count_overflow) {
        if (current_count == max_len) {
          max_count_overflow = true;
        } else {
          ++current_count;
          if (current_count > max_count) {
            max_count = current_count;
          }
        }
      }
    } else {
      current_count = 0;
    }

    curr_num = num;
    if (len_of_seq > 0 && curr_num % prev_num == 0) {
      if (!count_of_del_overflow) {
        if (count_of_del == max_len) {
          count_of_del_overflow = true;
        } else {
          ++count_of_del;
        }
      }
    }
    prev_num = curr_num;

    if (len_of_seq < max_len) {
      ++len_of_seq;
    }
  }

  if (std::cin.fail()) {
    std::cerr << "Ошибка ввода последовательности";
    return input_error;
  }

  if (max_count_overflow && count_of_del_overflow) {
    std::cerr << "Ошибка: Переполнение обоих счетчиков";
    return len_error;
  }

  if (max_count_overflow) {
    std::cout << "Характеристика №11: " << count_of_del << std::endl;
    return len_error;
  }

  if (count_of_del_overflow) {
    std::cout << "Характеристика №14: " << max_count << std::endl;
    return len_error;
  }

  std::cout << "Характеристика №14: " << max_count << std::endl;
  if (len_of_seq < min_len) {
    std::cerr << "Последовательность слишком короткая";
    return len_error;
  }

  std::cout << "Характеристика №11: " << count_of_del << std::endl;
  return good;
}
