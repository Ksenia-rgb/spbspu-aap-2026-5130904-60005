#include <iostream>

namespace usmanov
{
  int runLab1()
  {
    const int error_invalid_input = 1;
    const int error_too_short = 2;

    long long num = 0;
    long long min_element = 0;
    long long count_min = 0;
    bool has_elements = false;
    long long prev1 = 0;
    long long prev2 = 0;
    long long count_loc_min = 0;
    unsigned long long sequence_length = 0;
    bool has_zero = false;

    while (std::cin >> num)
    {
      if (num == 0)
      {
        has_zero = true;
        break;
      }

      if (!has_elements)
      {
        min_element = num;
        count_min = 1;
        has_elements = true;
      }
      else if (num < min_element)
      {
        min_element = num;
        count_min = 1;
      }
      else if (num == min_element)
      {
        count_min++;
      }

      sequence_length++;
      if (sequence_length >= 3)
      {
        if (prev1 < prev2 && prev1 < num)
        {
          count_loc_min++;
        }
      }
      prev2 = prev1;
      prev1 = num;
    }

    if (!has_zero || (!std::cin.eof() && std::cin.fail()))
    {
      std::cerr << "Ошибка: Некорректный ввод.\n";
      return error_invalid_input;
    }

    if (!has_elements)
    {
      std::cout << 0 << "\n";
    }
    else
    {
      std::cout << count_min << "\n";
    }

    if (sequence_length == 0)
    {
      std::cerr << "Ошибка: Мало элементов.\n";
      return error_too_short;
    }

    std::cout << count_loc_min << "\n";
    return 0;
  }
}

int main()
{
  return usmanov::runLab1();
}
