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

    bool len_overflow = false;
    bool count_overflow = false;

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
      }
      else
      {
        if (curr >= prev)
        {
          if (cur_len == std::numeric_limits< std::size_t >::max())
          {
            len_overflow = true;
          }
          else
          {
            ++cur_len;
            if (cur_len > best_len)
            {
              best_len = cur_len;
            }
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
          count_overflow = true;
        }
        else
        {
          ++min_count;
        }
      }

      prev = curr;
      first = false;
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

    if (len_overflow)
    {
      std::cerr << "overflow in length\n";
    }
    else
    {
      std::cout << best_len << "\n";
    }

    if (count_overflow)
    {
      std::cerr << "overflow in count\n";
    }
    else
    {
      std::cout << min_count << "\n";
    }

    if (len_overflow || count_overflow)
    {
      return kCalcError;
    }

    return kSuccess;
  }
}

int main()
{
  return nekhorukov::readSequence(std::cin);
}
