#include <iostream>
#include <limits>

int main()
{
  int n = std::numeric_limits< int >::min();
  int sub_max = 0, sub_dup = 0;
  int k_max = 0, i_1 = 0, i_2 = std::numeric_limits< int >::min(), i = 0, k_el = 0;
  int max = std::numeric_limits< int >::min();
  std::cout << "ввести значения: ";
  while (true)
  {
    std::cin >> n;

    if (!std::cin || n > std::numeric_limits< int >::max())
    {
      std::cout << "Некорректный ввод\n";
      return 1;
    }

    if (n == 0)
    {
      break;
    }

    k_el++;
    i = n;
    if (k_el == 1)
    {
      i_1 = n;
    }
    if (k_el == 2)
    {
      i_2 = n;
    }

    if (k_el > 2)
    {
      if (i_1 + i_2 == i)
      {
        sub_dup++;
      }
    }

    if (i_1 + i_2 == i)
    {
      ++sub_dup;
    }

    if (n == max)
    {
      k_max++;
    }
    if (max < n)
    {
      sub_max = max;
      max = n;
    }

    if (n > sub_max && n != max)
    {
      sub_max = n;
    }

    if (k_max > 0)
    {
      sub_max = max;
    }
  }

  if (k_el <= 2)
  {
    if (k_el == 1)

    {
      std::cout << "невозможно вычислить sub_dup\n";
      std::cout << "невозможно вычислить sub_max\n";
      return 2;
    }
    std::cout << "невозможно вычислить sub_dup\n" << "SUB_MAX == " << sub_max;
    return 2;
  }
  std::cout << "SUB_MAX == " << sub_max << "\n";
  std::cout << "sub_dup == " << sub_dup;
  return 0;
}
