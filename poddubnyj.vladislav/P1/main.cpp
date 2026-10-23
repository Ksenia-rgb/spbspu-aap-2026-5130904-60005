#include <cstddef>
#include <iostream>

namespace poddubnyj
{
const int SUCCESS = 0;
const int NOT_SEQ = 1;
const int IMPOSE_CALC = 2;

int resSeq()
{
  std::size_t cnt_min = 0;
  std::size_t cnt_max = 0;

  int x = 0;

  if (!(std::cin >> x))
  {
    std::cerr << "error: it is not sequence" << "\n";
    return NOT_SEQ;
  }

  if (x == 0)
  {
    std::cerr << "error: empty sequence" << "\n";
    return IMPOSE_CALC;
  }

  int curr_min = x;
  int curr_max = x;

  cnt_min++;
  cnt_max++;

  bool correct_complete = false;

  while (std::cin >> x)
  {
    if (x == 0)
    {
      correct_complete = true;
      break;
    }

    if (x == curr_min)
    {
      cnt_min++;
    }

    if (x == curr_max)
    {
      cnt_max++;
    }

    if (x < curr_min)
    {
      cnt_min = 1;
      curr_min = x;
    }

    if (x > curr_max)
    {
      cnt_max = 1;
      curr_max = x;
    }
  }

  if (!correct_complete)
  {
    if (std::cin.fail())
    {
      std::cerr << "error: it is not sequence" << "\n";
      return NOT_SEQ;
    }
  }

  std::cout << cnt_min << "\n";
  std::cout << cnt_max << "\n";
  return SUCCESS;
}
}

int main()
{
  return poddubnyj::resSeq();
}
