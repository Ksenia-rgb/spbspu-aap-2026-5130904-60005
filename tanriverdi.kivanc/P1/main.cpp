#include <iostream>

namespace tanriverdi
{
  struct Result
  {
    int inc_seq;
    int mon_inc;
  };

  bool readSequence(std::istream & in, Result & res)
  {
    res.inc_seq = 0;
    res.mon_inc = 0;

    int previous = 0;
    int current = 0;
    int length = 0;
    bool first = true;
    bool terminated = false;

    while (in >> current)
    {
      if (current == 0)
      {
        terminated = true;
        break;
      }

      if (first)
      {
        length = 1;
        res.mon_inc = 1;
        first = false;
      }
      else
      {
        if (current > previous)
        {
          ++res.inc_seq;
        }

        if (current >= previous)
        {
          ++length;
        }
        else
        {
          length = 1;
        }

        if (length > res.mon_inc)
        {
          res.mon_inc = length;
        }
      }

      previous = current;
    }

    return terminated;
  }
}

int main()
{
  tanriverdi::Result res;

  if (!tanriverdi::readSequence(std::cin, res))
  {
    std::cerr << "Error: input is not a valid sequence\n";
    return 1;
  }

  std::cout << res.inc_seq << '\n';
  std::cout << res.mon_inc << '\n';

  return 0;
}
