#include <iostream>
#include <limits>

namespace nekhorukov
{
  enum ErrorCode
  {
    kSuccess = 0,
    kInputError = 1,
    kCalcError = 2
  };

  int readSequence(std::istream & in)
  {
    int prev = 0;
    int curr = 0;

    bool first = true;

    int min_val = std::numeric_limits< int >::max();
    int min_count = 0;

    int cur_len = 0;
    int best_len = 0;

    while (in >> curr)
    {
      if (curr == 0)
      {
        break;
      }

      if (first)
      {
        cur_len = 1;
        best_len = 1;
        first = false;
      }
      else
      {
        if (curr >= prev)
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
      }

      if (curr < min_val)
      {
        min_val = curr;
        min_count = 1;
      }
      else if (curr == min_val)
      {
        ++min_count;
      }

      prev = curr;
    }

    if (in.fail())
    {
      std::cerr << "bad input\n";
      return kInputError;
    }

    if (first)
    {
      std::cout << "0\n";
      std::cout << "0\n";
      return kSuccess;
    }

    std::cout << best_len << "\n";
    std::cout << min_count << "\n";
    return kSuccess;
  }
}

int main()
{
  return nekhorukov::readSequence(std::cin);
}
