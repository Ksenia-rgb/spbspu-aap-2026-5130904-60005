#include <cstddef>
#include <iostream>
#include <limits>

int main()
{
  const size_t maxsize = std::numeric_limits< size_t >::max();
  int left = 0;
  if (!(std::cin >> left))
  {
    std::cerr << "Error: Incorrect data\n";
    return 1;
  }
  if (left == 0)
  {
    std::cout << "[EQL-SEQ] = " << 0 << '\n';
    std::cerr << "Error: sequence is too short to [LOC-MAX]\n";
    return 2;
  }
  int middle = 0;
  if (!(std::cin >> middle))
  {
    std::cerr << "Error: Incorrect data\n";
    return 1;
  }
  size_t locmaxcount = 0;
  size_t eqrunlen = 1;
  size_t maxeqrunlen = 1;
  if (middle != 0)
  {
    int right = 0;
    while (std::cin >> right)
    {
      if (left == middle)
      {
        if (eqrunlen == maxsize)
        {
          std::cerr << "Error: Overflow\n";
          return 2;
        }
        eqrunlen++;
      }
      else
      {
        if (maxeqrunlen < eqrunlen)
        {
          maxeqrunlen = eqrunlen;
        }
        eqrunlen = 1;
      }
      if (right == 0)
      {
        break;
      }
      if (left < middle && middle > right)
      {
        if (locmaxcount == maxsize)
        {
          std::cerr << "Error: Overflow\n";
          return 2;
        }
        locmaxcount++;
      }
      left = middle;
      middle = right;
    }
    if (std::cin.fail())
    {
      std::cerr << "Error: Incorrect data\n";
      return 1;
    }
  }
  if (maxeqrunlen < eqrunlen)
  {
    maxeqrunlen = eqrunlen;
  }
  std::cout << "[EQL-SEQ] = " << maxeqrunlen << '\n';
  std::cout << "[LOC-MAX] = " << locmaxcount << '\n';
  return 0;
}
