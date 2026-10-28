#include <iostream>
#include <limits>
#include <cstddef>

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
    std::size_t min_count = 0;

    std::size_t cur_len = 0;
    std::size_t best_len = 0;

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
          if (cur_len == std::numeric_limits< std::size_t >::max())
          {
            std::cerr << "overflow\n";
            return kCalcError;
          }
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
        if (min_count == std::numeric_limits< std::size_t >::max())
        {
          std::cerr << "overflow\n";
          return kCalcError;
        }
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
