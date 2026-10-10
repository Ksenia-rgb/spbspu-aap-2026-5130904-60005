#include <iostream>
#include <limits>
#include <cstddef>

int main()
{
  const std::size_t max_len = std::numeric_limits< size_t >::max();
  std::size_t len = 0;
  long long har11 = 0;
  long long har10 = 0;
  long long xl = 0;
  long long xm = 1;

  while (true)
  {
    long long x = 0;
    std::cin >> x;

    if (std::cin.fail())
    {
      std::cerr << "invalid argument" << std::endl;
      return 1;
    }

    if (len == max_len)
    {
      std::cerr << "too long" << std::endl;
      return 2;
    }
    if (x == 0)
    {
      break;
    }
    if (x % xm == 0 && len > 0)
    {
      har11++;
    }
    if (xm < xl && xm > x && len > 1)
    {
      har10++;
    }
    xl = xm;
    xm = x;
    len++;
  }

  if (len < 2)
  {
    std::cerr << "too short" << std::endl;
    return 2;
  }
  else
  {
    std::cout << "har 11: " << har11 << std::endl;
  }
  if (len < 3)
  {
    std::cerr << "too short for har10" << std::endl;
  }
  else
  {
    std::cout << "har 10: " << har10 << std::endl;
  }
  return 0;
}
