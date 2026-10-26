#include <cstddef>
#include <iostream>

namespace abdullaeva
{
  const int ERR_INVALID_INPUT = 1;
  const int ERR_TOO_SHORT = 2;

  const std::size_t MIN_COUNT_SUB_MAX = 2;
  const std::size_t MIN_COUNT_LOC_MAX = 1;
  const std::size_t MIN_COUNT_FOR_LOC_CHECK = 2;
  const std::size_t SECOND_ELEMENT_COUNT = 1;

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

      if (res.count >= MIN_COUNT_FOR_LOC_CHECK && prev > prev_prev && prev > x)
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
      else if (res.count == SECOND_ELEMENT_COUNT || x > res.sub_max)
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
  abdullaeva::Result res;
  if (!abdullaeva::readSequence(std::cin, res))
  {
    std::cerr << "Error: input is not a valid sequence\n";
    return abdullaeva::ERR_INVALID_INPUT;
  }

  int code = 0;

  if (res.count < abdullaeva::MIN_COUNT_SUB_MAX)
  {
    std::cerr << "Error: sequence is too short for SUB-MAX\n";
    code = abdullaeva::ERR_TOO_SHORT;
  }
  else
  {
    std::cout << res.sub_max << '\n';
  }

  if (res.count < abdullaeva::MIN_COUNT_LOC_MAX)
  {
    std::cerr << "Error: sequence is too short for LOC-MAX\n";
    code = abdullaeva::ERR_TOO_SHORT;
  }
  else
  {
    std::cout << res.loc_max << '\n';
  }

  return code;
}
