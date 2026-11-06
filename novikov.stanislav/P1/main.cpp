#include <iostream>
#include <limits>

namespace novikov
{
  bool addChecked(unsigned long long& value, unsigned long long addend)
  {
    if (value > std::numeric_limits< unsigned long long >::max() - addend)
    {
      return false;
    }
    value += addend;
    return true;
  }
}

int main()
{
  const int code_invalid_input = 1;
  const int code_calculation_error = 2;
  const unsigned long long initial_len = 1;
  const long long even_divisor = 2;

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
  bool mon_overflow = false;
  bool even_overflow = false;

  while ((std::cin >> current) && (current != 0))
  {
    if (!mon_overflow)
    {
      if (current <= prev)
      {
        if (!novikov::addChecked(mon_cur, initial_len))
        {
          mon_overflow = true;
        }
      }
      else
      {
        mon_cur = initial_len;
      }

      if (!mon_overflow && mon_cur > mon_max)
      {
        mon_max = mon_cur;
      }
    }

    if (!even_overflow)
    {
      if (current % even_divisor == 0)
      {
        if (!novikov::addChecked(even_cur, initial_len))
        {
          even_overflow = true;
        }
      }
      else
      {
        even_cur = 0;
      }

      if (!even_overflow && even_cur > even_max)
      {
        even_max = even_cur;
      }
    }

    prev = current;
  }

  if (std::cin.fail())
  {
    std::cerr << "Error: Invalid input. Please enter a valid integer.\n";
    return code_invalid_input;
  }

  if (!mon_overflow)
  {
    std::cout << mon_max << "\n";
  }
  else
  {
    std::cerr << "Error: Overflow occurred while calculating monotonic length.\n";
  }

  if (!even_overflow)
  {
    std::cout << even_max << "\n";
  }
  else
  {
    std::cerr << "Error: Overflow occurred while calculating even length.\n";
  }

  if (mon_overflow || even_overflow)
  {
    return code_calculation_error;
  }

  return 0;
}
