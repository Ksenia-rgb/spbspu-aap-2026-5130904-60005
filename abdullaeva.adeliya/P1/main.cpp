#include <cstddef>
#include <iostream>

namespace adel
{
  struct Result
  {
    std::size_t count;
    int sub_max;
    std::size_t loc_max;
  };

  bool readSequence(std::istream & in, Result & res)
  {
    res.count = 0;
    res.sub_max = 0;
    res.loc_max = 0;

    int max = 0;
    int prev_prev = 0;
    int prev = 0;
    int x = 0;

    while (in >> x)
    {
      if (x == 0)
      {
        return true;
      }

      if (res.count >= 2 && prev > prev_prev && prev > x)
      {
        ++res.loc_max;
      }

      if (res.count == 0)
      {
        max = x;
      }
      else if (x > max)
      {
        res.sub_max = max;
        max = x;
      }
      else if (res.count == 1 || x > res.sub_max)
      {
        res.sub_max = x;
      }

      prev_prev = prev;
      prev = x;
      ++res.count;
    }
    return false;
  }
}

int main()
{
  adel::Result res;
  if (!adel::readSequence(std::cin, res))
  {
    std::cerr << "Error: input is not a valid sequence\n";
    return 1;
  }

  int code = 0;

  if (res.count < 2)
  {
    std::cerr << "Error: sequence is too short for SUB-MAX\n";
    code = 2;
  }
  else
  {
    std::cout << res.sub_max << '\n';
  }

  if (res.count < 1)
  {
    std::cerr << "Error: sequence is too short for LOC-MAX\n";
    code = 2;
  }
  else
  {
    std::cout << res.loc_max << '\n';
  }

  return code;
}
