#include <iostream>
#include <cstdlib>

namespace usmanov
{
  void runLab1()
  {
    long long num = 0;
    long long min_element = 0;
    long long count_min = 0;
    bool has_elements = false;
    long long prev1 = 0;
    long long prev2 = 0;
    long long count_loc_min = 0;
    unsigned long long sequence_length = 0;

    while (std::cin >> num)
    {
      if (num == 0)
      {
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

    if (!std::cin.eof() && std::cin.fail())
    {
      std::cerr << "Ошибка: Некорректный ввод.\n";
      std::exit(1);
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
      std::exit(2);
    }

    std::cout << count_loc_min << "\n";
  }
}

int main()
{
  usmanov::runLab1();
  return 0;
}
