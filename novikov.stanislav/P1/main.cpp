#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace novikov
{
  unsigned long long addChecked(unsigned long long first, unsigned long long second)
  {
    if (first > std::numeric_limits< unsigned long long >::max() - second)
    {
      throw std::overflow_error("Overflow occurred during addition.");
    }
    return first + second;
  }
}

int main()
{
  const int code_invalid_input = 1;
  const int code_calculation_error = 2;
  const unsigned long long initial_len = 1;
  const long long even_divisor = 2;

  try
  {
    long long current = 0;
    if (!(std::cin >> current))
    {
      std::cerr << "Error: Invalid input. Please enter a valid integer.\n";
      return code_invalid_input;
    }

    if (current == 0)
    {
      std::cout << 0 << "\n" << 0 << "\n";
      return 0;
    }

    unsigned long long mon_cur = initial_len;
    unsigned long long mon_max = initial_len;
    unsigned long long even_cur = (current % even_divisor == 0) ? initial_len : 0;
    unsigned long long even_max = even_cur;
    long long prev = current;

    while ((std::cin >> current) && (current != 0))
    {
      if (current <= prev)
      {
        mon_cur = novikov::addChecked(mon_cur, initial_len);
      }
      else
      {
        mon_cur = initial_len;
      }

      if (mon_cur > mon_max)
      {
        mon_max = mon_cur;
      }

      if (current % even_divisor == 0)
      {
        even_cur = novikov::addChecked(even_cur, initial_len);
      }
      else
      {
        even_cur = 0;
      }

      if (even_cur > even_max)
      {
        even_max = even_cur;
      }

      prev = current;
    }

    if (std::cin.fail())
    {
      std::cerr << "Error: Invalid input. Please enter a valid integer.\n";
      return code_invalid_input;
    }

    std::cout << mon_max << "\n" << even_max << "\n";
  }
  catch (const std::exception& error)
  {
    std::cerr << error.what() << "\n";
    return code_calculation_error;
  }

  return 0;
}
