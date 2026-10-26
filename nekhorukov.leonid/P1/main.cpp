#include <iostream>
#include <climits>

namespace nekhorukov
{
  int readSequence(std::istream & in)
  {
    int prev = 0;
    int curr = 0;
    bool first = true;
    bool any = false;
    bool bad = false;

    int minVal = INT_MAX;
    int minCount = 0;

    int curLen = 0;
    int bestLen = 0;

    while (in >> curr)
    {
      if (curr == 0)
      {
        break;
      }

      any = true;

      if (curr < minVal)
      {
        minVal = curr;
        minCount = 1;
      }
      else if (curr == minVal)
      {
        ++minCount;
      }

      if (first)
      {
        curLen = 1;
        bestLen = 1;
        first = false;
      }
      else if (curr >= prev)
      {
        ++curLen;
        if (curLen > bestLen)
        {
          bestLen = curLen;
        }
      }
      else
      {
        curLen = 1;
      }

      prev = curr;
    }

    if (in.fail() && !in.eof())
    {
      bad = true;
    }

    if (bad)
    {
      std::cerr << "bad input\n";
      return 1;
    }

    if (!any)
    {
      std::cout << "0\n";
      std::cout << "0\n";
      return 0;
    }

    std::cout << bestLen << "\n";
    std::cout << minCount << "\n";
    return 0;
  }
}

int main()
{
  return nekhorukov::readSequence(std::cin);
}
