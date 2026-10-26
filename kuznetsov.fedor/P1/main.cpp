#include <iostream>
#include <limits>

int main()
{
  const int cin_err = 1, unreal_par = 2;
  int n = std::numeric_limits< int >::min();
  int sub_max = 0, sub_dup = 0;
  int k_max = 0, first_num = 0, second_num = std::numeric_limits< int >::min(), actual_num = 0, k_el = 0;
  int max = std::numeric_limits< int >::min();
  while (std::cin >> n)
  {
    if (n == 0)
    {
      break;
    }

    k_el++;
    actual_num = n;
    if (k_el == 1)
    {
      first_num = n;
    }
    else if (k_el == 2)
    {
      second_num = n;
    }

    if (k_el > 2)
    {
      if (first_num + second_num == actual_num)
      {
        sub_dup++;
      }
      first_num = second_num;
      second_num = n;
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
  if (!std::cin)
  {
    std::cerr << "Некорректный ввод\n";
    return cin_err;
  }

  if (k_el <= 2)
  {
    if (k_el == 1 || k_el == 0)

    {
      std::cerr << "невозможно вычислить sub_dup\n";
      std::cerr << "невозможно вычислить sub_max\n";
      return unreal_par;
    }
    std::cerr << "невозможно вычислить sub_dup\n";
    std::cout << sub_max << "\n";
    return unreal_par;
  }
  std::cout << sub_max << "\n";
  std::cout << sub_dup << "\n";
  return 0;
}
