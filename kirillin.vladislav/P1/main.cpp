#include <iostream>
#include <limits>
#include <cstddef>

int main()
{
  const int code_good = 0;
  const int code_invalid_input = 1;
  const int code_short = 2;
  std::size_t max_len = std::numeric_limits< size_t >::max();
  std::size_t len = 0;
  long long har11 = 0;
  long long har10 = 0;
  long long x_left = 0;
  long long x_mid = 1;
  long long x_right = 0;

  while (std::cin >> x_right)
  {
    if (x_right == 0)
    {
      break;
    }
    if (x_right % x_mid == 0 && len > 0)
    {
      har11++;
    }
    if (x_mid < x_left && x_mid > x_right && len > 1)
    {
      har10++;
    }

    x_left = x_mid;
    x_mid = x_right;

    if (len == max_len)
    {
      len = len;
    }
    else
    {
      len++;
    }
  }

  if (std::cin.fail() && !std::cin.eof())
  {
    std::cerr << "invalid argument" << std::endl;
    return code_invalid_input;
  }

  if (len < 2)
  {
    std::cerr << "too short" << std::endl;
    return code_short;
  }

  std::cout << "har 11: " << har11 << std::endl;

  if (len < 3)
  {
    std::cerr << "too short for har10" << std::endl;
    return code_short;
  }

  std::cout << "har 10: " << har10 << std::endl;

  return code_good;
}
