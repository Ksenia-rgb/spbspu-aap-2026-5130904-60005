#include <iostream>
#include <climits>
#include <cstdlib>

namespace nekhorukov
{
  int read_sequence(std::istream & in)
  {
    int prev = 0;
    int curr = 0;
    bool first = true;
    bool any = false;
    bool bad = false;

    int min_val = INT_MAX;
    int min_count = 0;

    int cur_len = 0;
    int best_len = 0;

    while (in >> curr)
    {
      if (curr == 0)
      {
        break;
      }

      any = true;

      if (curr < min_val)
      {
        min_val = curr;
        min_count = 1;
      }
      else if (curr == min_val)
      {
        ++min_count;
      }

      if (first)
      {
        cur_len = 1;
        best_len = 1;
        first = false;
      }
      else if (curr >= prev)
      {
        ++cur_len;
        if (cur_len > best_len)
        {
          best_len = cur_len;
        }
      }
      else
      {
        cur_len = 1;
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

    std::cout << best_len << "\n";
    std::cout << min_count << "\n";
    return 0;
  }
}

int main()
{
  return nekhorukov::read_sequence(std::cin);
}
